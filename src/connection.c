#include <pthread.h>
#include <unistd.h>

#include <mwebd/hashmap.h>
#include <mwebd/http.h>

#include "connection.h"

_Thread_local static int connection;
_Thread_local static char request_buf[16384];
_Thread_local static size_t request_size;

static void read_request() {
  char ch;
  request_size = 0;
  
  unsigned state = 0;
  
  while (true) {
    if (request_size == 16384) {
      http_default_response(connection, STATUS_413);
      close(connection);
      pthread_exit(NULL);
    }
    
    if (read(connection, &ch, 1) != 1) {
      close(connection);
      pthread_exit(NULL);
    }
    
    request_buf[request_size] = ch;
    request_size++;
    
    switch (state) {
      case 0:
        if (ch == '\r') {
          state++;
        }
      break;
      case 1:
        if (ch == '\n') {
          state++;
        } else {
          state = 0;
        }
      break;
      case 2:
        if (ch == '\r') {
          state++;
        } else {
          state = 0;
        }
      break;
      case 3:
        if (ch == '\n') {
          return;
        } else {
          state = 0;
        }
      break;
    }
  }
}

void* connection_main(void* connection_raw) {
  connection = (long)connection_raw;
  
  method_t method;
  char* uri;
  hashmap_t* request_headers;
  
  read_request();
  
  http_default_response(connection, http_parse(request_buf, request_size, &method, &uri, &request_headers));
  
  close(connection);
  pthread_exit(NULL);
}
