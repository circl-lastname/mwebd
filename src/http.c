#include <unistd.h>

#include <mwebd/http.h>

#include "default_responses.h"

status_t http_parse(char* request_buf, size_t request_size, method_t* method, char** uri, hashmap_t** hashmap) {
  
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
