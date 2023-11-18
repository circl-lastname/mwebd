#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mwebd/config.h>
#include <mwebd/hashmap.h>
#include <mwebd/log.h>

static hashmap_t* config_hashmap;

static FILE* config_file;
static int ch;

static void consume() {
  ch = fgetc(config_file);
}

static void error(char* string) {
  log_crit("Failed to parse config file: %s\n", string);
  exit(1);
}

static char* read_name() {
  char* string = malloc(32);
  if (!string) {
    error("Failed to allocate string buffer");
  }
  
  size_t string_size = 32;
  
  unsigned i = 0;
  
  while (true) {
    switch (ch) {
      case '=':
        consume();
        goto break_loop;
      break;
      case EOF:
        error("Unexpected end of file in key name");
      break;
    }
    
    string[i] = ch;
    i++;
    consume();
    
    if (i >= string_size) {
      string_size *= 2;
      string = realloc(string, string_size);
      if (!string) {
        error("Failed to allocate string buffer");
      }
    }
  }
  break_loop:
  
  string[i] = '\0';
  
  return string;
}

static char* read_value() {
  char* string = malloc(32);
  if (!string) {
    error("Failed to allocate string buffer");
  }
  
  size_t string_size = 32;
  
  unsigned i = 0;
  
  while (true) {
    switch (ch) {
      case '\n':
        consume();
        goto break_loop;
      break;
      case '\r':
        consume();
        continue;
      break;
      case EOF:
        goto break_loop;
      break;
    }
    
    string[i] = ch;
    i++;
    consume();
    
    if (i >= string_size) {
      string_size *= 2;
      string = realloc(string, string_size);
      if (!string) {
        error("Failed to allocate string buffer");
      }
    }
  }
  break_loop:
  
  string[i] = '\0';
  
  return string;
}

void config_load(char* filename) {
  config_file = fopen(filename, "r");
  if (!config_file) {
    error(strerror(errno));
  }
  
  config_hashmap = hashmap_create();
  if (!config_hashmap) {
    error("Failed to allocate hashmap");
  }
  
  consume();
  
  while (true) {
    char* name = read_name();
    char* value = read_value();
    
    if (!hashmap_add(config_hashmap, (unsigned char*)name, strlen(name), free, value, free)) {
      error("Failed to save value to hashmap");
    }
    
    if (ch == EOF) {
      break;
    }
  }
  
  fclose(config_file);
}

char* config_get(char* name) {
  return hashmap_get(config_hashmap, (unsigned char*)name, strlen(name));
}
