#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mwebd/http.h>

#include "default_responses.h"

#define NEXT \
  i++; \
  if (i == request_size) { \
    return STATUS_400; \
  }

static bool parse_hex(char input, uint8_t* output) {
  if (input >= '0' && input <= '9') {
    *output = input - 0x30;
  } else if (input >= 'a' && input <= 'f') {
    *output = input - 0x61 + 10;
  } else if (input >= 'A' && input <= 'F') {
    *output = input - 0x41 + 10;
  } else {
    return false;
  }
  
  return true;
}

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
  
  if (i >= request_size) {
    return STATUS_400;
  }
  
  {
    *uri = malloc(64);
    if (!*uri) {
      return STATUS_500;
    }
    
    size_t uri_size = 32;
    
    unsigned j = 0;
    
    while (true) {
      switch (request_buf[i]) {
        case ' ':
          NEXT
          goto break_loop;
        break;
        case '%': {
          uint8_t n1;
          uint8_t n2;
          
          NEXT
          if (!parse_hex(request_buf[i], &n1)) {
            return STATUS_400;
          }
          
          NEXT
          if (!parse_hex(request_buf[i], &n2)) {
            return STATUS_400;
          }
          
          uint8_t n = (n1 << 4) | n2;
          
          if (n != '\0') {
            (*uri)[j] = n;
            j++;
          }
        } break;
        default:
          (*uri)[j] = request_buf[i];
          j++;
        break;
      }
      
      NEXT
      
      if (j >= uri_size) {
        uri_size *= 2;
        *uri = realloc(*uri, uri_size);
        if (!*uri) {
          return STATUS_500;
        }
      }
    }
    break_loop:
    
    (*uri)[j] = '\0';
  }
  
  if (i+10 >= request_size) {
    return STATUS_400;
  }
  
  if (memcmp(&request_buf[i], "HTTP/1.1\r\n", 10)) {
    return STATUS_505;
  }
  
  i += 10;
  
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
