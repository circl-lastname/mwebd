#pragma once

#include <mwebd/hashmap.h>

#define ENUMERATE_STATUSES(f) \
  f(200) \
  f(413) \
  f(500) \
  f(501)

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

typedef enum {
  #define F(n) STATUS_##n,
  ENUMERATE_STATUSES(F)
  #undef F
} status_t;

status_t http_parse(char* request_buf, size_t request_size, method_t* method, char** uri, hashmap_t** hashmap);
void http_default_response(int connection, status_t status);
