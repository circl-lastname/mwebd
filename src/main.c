#include <stdio.h>

#include <mwebd/config.h>
#include <mwebd/log.h>

#include "listener.h"

int main(int argc, char** argv) {
  log_info("MWebD - Modular HTTP server and backend runtime\n");
  
  if (argc != 2) {
    log_crit("Invalid amount of arguments, usage: mwebd <config_file>\n");
    return 1;
  }
  
  config_load(argv[1]);
  log_info("Loaded config %s\n", argv[1]);
  
  listener_start();
  
  return 0;
}
