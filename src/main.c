#include <stdio.h>

#include <mwebd/config.h>
#include <mwebd/log.h>

int main(int argc, char** argv) {
  log_info("MWebD - Modular HTTP server and backend runtime\n");
  
  if (argc != 2) {
    log_crit("Invalid amount of arguments, usage: mwebd <config_file>\n");
    return 1;
  }
  
  config_load(argv[1]);
  
  return 0;
}
