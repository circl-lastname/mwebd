#include <pthread.h>
#include <unistd.h>

#include "connection.h"

void* connection_main(void* connection_raw) {
  int connection = (long)connection_raw;
  
  close(connection);
  pthread_exit(NULL);
}
