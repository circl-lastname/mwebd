#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

#include <mwebd/log.h>

static bool stderr_isatty;

void log_init() {
  if (isatty(STDERR_FILENO)) {
    stderr_isatty = true;
  } else {
    stderr_isatty = false;
  }
}

void log_info(char* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  
  if (stderr_isatty) {
    vfprintf(stderr, format, arguments);
  } else {
    fputs("[INFO] ", stderr);
    vfprintf(stderr, format, arguments);
  }
  
  va_end(arguments);
}

void log_warn(char* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  
  if (stderr_isatty) {
    fputs("\x1b[93m", stderr);
    vfprintf(stderr, format, arguments);
    fputs("\x1b[0m", stderr);
  } else {
    fputs("[WARN] ", stderr);
    vfprintf(stderr, format, arguments);
  }
  
  va_end(arguments);
}

void log_crit(char* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  
  if (stderr_isatty) {
    fputs("\x1b[31m", stderr);
    vfprintf(stderr, format, arguments);
    fputs("\x1b[0m", stderr);
  } else {
    fputs("[CRIT] ", stderr);
    vfprintf(stderr, format, arguments);
  }
  
  va_end(arguments);
}
