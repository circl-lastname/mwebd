#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef struct hashmap hashmap_t;
typedef void free_func_t(void*);

hashmap_t* hashmap_create();
bool hashmap_add(hashmap_t* hashmap, uint8_t* name, size_t name_size, free_func_t* name_free_func, void* data, free_func_t* data_free_func);
void* hashmap_get(hashmap_t* hashmap, uint8_t* name, size_t name_size);
void hashmap_free(hashmap_t* hashmap);
