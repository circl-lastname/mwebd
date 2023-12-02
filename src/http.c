#include <string.h>
#include <unistd.h>

#include <mwebd/http.h>

#include "default_responses.h"

status_t http_parse(char* request_buf, size_t request_size, method_t* method, char** uri, hashmap_t** hashmap) {
  unsigned i = 0;
  
  if (!memcmp(request_buf, "GET ", 4)) {
    *method = METHOD_GET;
    i += 4;
  } else if (!memcmp(request_buf, "HEAD ", 5)) {
    *method = METHOD_HEAD;
    i += 5;
  } else if (!memcmp(request_buf, "POST ", 5)) {
    *method = METHOD_POST;
    i += 5;
  } else if (!memcmp(request_buf, "PUT ", 4)) {
    *method = METHOD_PUT;
    i += 4;
  } else if (!memcmp(request_buf, "DELETE ", 7)) {
    *method = METHOD_DELETE;
    i += 7;
  } else if (!memcmp(request_buf, "CONNECT ", 8)) {
    *method = METHOD_CONNECT;
    i += 8;
  } else if (!memcmp(request_buf, "OPTIONS ", 8)) {
    *method = METHOD_OPTIONS;
    i += 8;
  } else if (!memcmp(request_buf, "TRACE ", 6)) {
    *method = METHOD_TRACE;
    i += 6;
  } else {
    return STATUS_501;
  }
  
  return STATUS_200;
}

void http_default_response(int connection, status_t status) {
  char* response;
  size_t response_size;
  
  switch (status) {
    #define F(n) \
      case STATUS_##n: \
        response = response_##n; \
        response_size = sizeof(response_##n)-1; \
      break;
    ENUMERATE_STATUSES(F)
    #undef F
    default:
      response = response_500;
      response_size = sizeof(response_500)-1;
    break;
  }
  
  write(connection, response, response_size);
}
