#include <errno.h>
#include <netinet/ip.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <mwebd/config.h>
#include <mwebd/log.h>

#include "listener.h"

void listener_start() {
  int main_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  
  if (main_socket < 0) {
    log_crit("Failed to create socket: %s\n", strerror(errno));
    exit(1);
  }
  
  uint16_t port;
  char* port_string = config_get("core.http.port");
  
  if (!port_string) {
    log_warn("HTTP port not specified in config, using 8000\n");
    port = 8000;
  } else {
    port = atoi(port_string);
  }
  
  struct sockaddr_in address = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr = { INADDR_ANY },
  };
  
  while (bind(main_socket, (struct sockaddr*)&address, sizeof(struct sockaddr_in)) < 0) {
    if (errno == EADDRINUSE) {
      log_warn("Failed to bind socket: %s, retrying...\n", strerror(errno));
      sleep(1);
    } else {
      log_crit("Failed to bind socket: %s\n", strerror(errno));
      exit(1);
    }
  }
  
  if (listen(main_socket, 256) < 0) {
    log_crit("Failed to start listening: %s\n", strerror(errno));
    exit(1);
  }
  
  log_info("Listening on port %u...\n", port);
  
  while (true) {
    int connection = accept(main_socket, NULL, NULL);
    
    if (connection < 0) {
      log_warn("Failed to accept connection: %s\n", strerror(errno));
      continue;
    }
    
    close(connection);
  }
}
