#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <mwebd/endpoint.h>
#include <mwebd/hashmap.h>
#include <mwebd/log.h>

static hashmap_t* endpoint_hashmap;

void endpoint_init() {
  endpoint_hashmap = hashmap_create();
  
  if (!endpoint_hashmap) {
    log_crit("Failed to create endpoint hashmap\n");
    exit(1);
  }
}

void endpoint_register(char* name, endpoint_t* endpoint) {
  if (!hashmap_add(endpoint_hashmap, (unsigned char*)name, strlen(name), NULL, endpoint, NULL)) {
    log_crit("Failed to register endpoint %s\n", name);
    exit(1);
  }
}

bool endpoint_get(char* name, void** content, size_t* size) {
  endpoint_t* endpoint = hashmap_get(endpoint_hashmap, (unsigned char*)name, strlen(name));
  
  if (!endpoint) {
    return false;
  }
  
  if (endpoint->type == ENDPOINT_STATIC) {
    *size = endpoint->static_size;
    *content = endpoint->static_content;
    return true;
  } else if (endpoint->type == ENDPOINT_GENERATED) {
    return endpoint->generated_func(content, size);
  }
  
  return false;
}
