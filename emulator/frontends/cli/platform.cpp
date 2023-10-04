#include "platform.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>

void omam8::Platform::serialPut(const char c) { putchar(c); }
void omam8::Platform::debugConsolePrint(const char *fmt, ...) {
  char buf[2048];
  memset(buf, 0, 2048);

  va_list ap;
  va_start(ap, fmt);
  vsnprintf(buf, 2048, fmt, ap);
  va_end(ap);

  fputs(buf, stderr);
  fputc('\n', stderr);
}

void omam8::Platform::dumpCoreState() {
  // TODO: copy-paste from old code
}
