#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

#include <mwebd/log.h>

void log_info(char* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  
  vfprintf(stderr, format, arguments);
  
  va_end(arguments);
}

void log_warn(char* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  
  if (isatty(STDERR_FILENO)) {
    fputs("\x1b[93m", stderr);
    vfprintf(stderr, format, arguments);
    fputs("\x1b[0m", stderr);
  } else {
    vfprintf(stderr, format, arguments);
  }
  
  va_end(arguments);
}

void log_crit(char* format, ...) {
  va_list arguments;
  va_start(arguments, format);
  
  if (isatty(STDERR_FILENO)) {
    fputs("\x1b[31m", stderr);
    vfprintf(stderr, format, arguments);
    fputs("\x1b[0m", stderr);
  } else {
    vfprintf(stderr, format, arguments);
  }
  
  va_end(arguments);
}
