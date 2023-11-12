#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hashmap.h"

static uint8_t hash_xor[] = {
  170,226,22, 201,181,127,246,82, 0,  217,82, 121,130,194,165,167,
  32, 225,164,0,  98, 188,138,28, 77, 15, 185,12, 236,182,101,97,
  4,  196,110,230,192,66, 103,118,195,154,93, 72, 177,26, 48, 15,
  217,173,181,51, 191,138,62, 126,53, 4,  69, 37, 1,  144,87, 136,
  58, 157,242,6,  25, 178,76, 82, 32, 150,222,97, 242,42, 185,83,
  73, 26, 237,63, 107,144,4,  165,84, 249,66, 22, 86, 61, 164,186,
  101,128,52, 78, 224,107,180,168,57, 120,238,51, 166,98, 32, 112,
  113,175,4,  14, 124,64, 159,160,155,47, 158,227,2,  232,94, 103,
  222,225,29, 167,142,108,0,  159,231,122,25, 30, 106,121,11, 239,
  64, 99, 86, 249,27, 55, 72, 222,124,223,175,93, 90, 187,187,74,
  201,184,241,19, 242,101,157,178,218,1,  106,64, 61, 36, 226,134,
  27, 247,12, 97, 118,24, 46, 27, 59, 11, 80, 105,156,57, 235,240,
  108,186,160,142,84, 22, 180,239,27, 53, 244,108,121,234,44, 90,
  125,41, 114,75, 238,148,254,112,42, 228,184,184,254,10, 75, 109,
  243,159,110,181,67, 97, 155,49, 61, 96, 37, 64, 51, 211,57, 213,
  185,138,253,223,135,28, 197,71, 227,164,246,198,228,49, 20, 135
};

struct hashmap_option {
  uint8_t* name;
  size_t name_size;
  void* data;
  free_func_t* free_func;
};

struct hashmap_options {
  unsigned amount;
  struct hashmap_option options[];
};

struct hashmap_layer_2 {
  struct hashmap_options* b2[256];
};

struct hashmap {
  struct hashmap_layer_2* b1[256];
};

hashmap_t* hashmap_create() {
  return calloc(1, sizeof(hashmap_t));
}

bool hashmap_add(hashmap_t* hashmap, uint8_t* name, size_t name_size, void* data, free_func_t* free_func) {
  uint16_t hash = 0;
  
  for (unsigned i = 0; i < name_size; i += 2) {
    if (name_size < i+2) {
      hash ^= (name[i] ^ hash_xor[i % 256]);
    } else {
      hash ^= (name[i] ^ hash_xor[i % 256]) | ((name[i+1] ^ hash_xor[(i+1) % 256]) << 8);
    }
  }
  
  uint8_t hash_b1 = hash;
  uint8_t hash_b2 = hash >> 8;
  
  struct hashmap_layer_2* layer_2 = hashmap->b1[hash_b1];
  
  if (!layer_2) {
    layer_2 = calloc(1, sizeof(struct hashmap_layer_2));
    
    if (!layer_2) {
      return false;
    }
    
    hashmap->b1[hash_b1] = layer_2;
  }
  
  struct hashmap_options* options = layer_2->b2[hash_b2];
  
  if (!options) {
    options = malloc(sizeof(struct hashmap_options) + sizeof(struct hashmap_option));
    
    if (!options) {
      return false;
    }
    
    layer_2->b2[hash_b2] = options;
    
    options->amount = 1;
    options->options[0].name = name;
    options->options[0].name_size = name_size;
    options->options[0].data = data;
    options->options[0].free_func = free_func;
  } else {
    options = realloc(options, sizeof(struct hashmap_options) + (options->amount+1)*sizeof(struct hashmap_option));
    
    if (!options) {
      return false;
    }
    
    layer_2->b2[hash_b2] = options;
    
    options->options[options->amount].name = name;
    options->options[options->amount].name_size = name_size;
    options->options[options->amount].data = data;
    options->options[options->amount].free_func = free_func;
    options->amount++;
  }
  
  return true;
}

void* hashmap_get(hashmap_t* hashmap, uint8_t* name, size_t name_size) {
  uint16_t hash = 0;
  
  for (unsigned i = 0; i < name_size; i += 2) {
    if (name_size < i+2) {
      hash ^= (name[i] ^ hash_xor[i % 256]);
    } else {
      hash ^= (name[i] ^ hash_xor[i % 256]) | ((name[i+1] ^ hash_xor[(i+1) % 256]) << 8);
    }
  }
  
  uint8_t hash_b1 = hash;
  uint8_t hash_b2 = hash >> 8;
  
  struct hashmap_layer_2* layer_2 = hashmap->b1[hash_b1];
  
  if (!layer_2) {
    return NULL;
  }
  
  struct hashmap_options* options = layer_2->b2[hash_b2];
  
  if (!options) {
    return NULL;
  }
  
  for (unsigned i = 0; i < options->amount; i++) {
    if (options->options[i].name_size != name_size) {
      continue;
    }
    
    if (!memcmp(options->options[i].name, name, name_size)) {
      return options->options[i].data;
    }
  }
  
  return NULL;
}

void hashmap_free(hashmap_t* hashmap) {
  for (unsigned i = 0; i < 256; i++) {
    struct hashmap_layer_2* layer_2 = hashmap->b1[i];
    
    if (layer_2) {
      for (unsigned j = 0; j < 256; j++) {
        struct hashmap_options* options = layer_2->b2[j];
        
        if (options) {
          for (unsigned k = 0; k < options->amount; k++) {
            if (options->options[k].free_func) {
              options->options[k].free_func(options->options[k].data);
            }
          }
          
          free(options);
        }
      }
      
      free(layer_2);
    }
  }
  
  free(hashmap);
}
