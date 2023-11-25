#include <dlfcn.h>
#include <stdbool.h>
#include <stdlib.h>

#include <mwebd/config.h>
#include <mwebd/log.h>

#include "module.h"

static char buf[1024];

static void load_module() {
  void* dl_handle = dlopen(buf, RTLD_NOW);
  
  if (!dl_handle) {
    log_crit("Failed to load module: %s\n", dlerror());
    return;
  }
  
  void (*init_function)() = dlsym(dl_handle, "mwebd_module_main");
  
  if (!init_function) {
    log_crit("Failed to load module: %s\n", dlerror());
    dlclose(dl_handle);
    return;
  }
  
  init_function();
  
  log_info("Loaded module %s\n", buf);
}

void module_init() {
  char* modules_string = config_get("modules");
  
  if (!modules_string) {
    log_warn("No modules key specified in config\n");
    return;
  }
  
  unsigned i = 0;
  unsigned buf_i = 0;
  
  while (true) {
    switch (modules_string[i]) {
      case '\0':
        buf[buf_i] = '\0';
        load_module();
        return;
      break;
      case ',':
        buf[buf_i] = '\0';
        buf_i = 0;
        i++;
        load_module();
        continue;
      break;
      case '\\':
        i++;
        buf[buf_i] = modules_string[i];
        
        if (modules_string[i] == '\0') {
          load_module();
          return;
        }
      break;
      default:
        buf[buf_i] = modules_string[i];
      break;
    }
    
    buf_i++;
    i++;
    
    if (buf_i == 1024) {
      log_crit("Module filename too long\n");
      return;
    }
  }
}
