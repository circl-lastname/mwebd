#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct hashmap hashmap_t;
typedef void free_func_t(void*);

hashmap_t* hashmap_create();
bool hashmap_add(hashmap_t* hashmap, uint8_t* name, size_t name_size, void* data, free_func_t* free_func);
void* hashmap_get(hashmap_t* hashmap, uint8_t* name, size_t name_size);
void hashmap_free(hashmap_t* hashmap);
