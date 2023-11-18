#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef bool endpoint_func_t(void** content, size_t* size);

typedef enum {
  ENDPOINT_STATIC,
  ENDPOINT_GENERATED,
} endpoint_type_t;

typedef struct {
  endpoint_type_t type;
  union {
    size_t static_size;
    endpoint_func_t* generated_func;
  };
  
  uint8_t static_content[];
} endpoint_t;

void endpoint_init();
void endpoint_register(char* name, endpoint_t* endpoint);
bool endpoint_get(char* name, void** content, size_t* size);
