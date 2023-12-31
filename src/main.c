#include <mwebd/config.h>
#include <mwebd/endpoint.h>
#include <mwebd/log.h>

#include "listener.h"
#include "module.h"

int main(int argc, char** argv) {
  log_init();
  
  log_info("MWebD - Modular HTTP server and backend runtime\n");
  
  if (argc != 2) {
    log_crit("Invalid amount of arguments, usage: mwebd <config_file>\n");
    return 1;
  }
  
  config_init(argv[1]);
  log_info("Loaded config %s\n", argv[1]);
  endpoint_init();
  module_init();
  listener_start();
  
  return 0;
}
