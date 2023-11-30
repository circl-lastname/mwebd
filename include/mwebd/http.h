#pragma once

#include <mwebd/hashmap.h>

typedef enum {
  METHOD_GET,
  METHOD_HEAD,
  METHOD_POST,
  METHOD_PUT,
  METHOD_DELETE,
  METHOD_CONNECT,
  METHOD_OPTIONS,
  METHOD_TRACE,
} method_t;

unsigned http_parse(char* request_buf, size_t request_size, method_t* method, char** uri, hashmap_t** hashmap);
