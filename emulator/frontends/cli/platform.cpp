#include "platform.h"
#include "core.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>

void omam8::Platform::serialPut(const char c) { putchar(c); }
void omam8::Platform::debugConsolePrint(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vfprintf(stderr, fmt, ap);
  va_end(ap);

  fputc('\n', stderr);
}

void omam8::Platform::dumpCoreState(bool io_pins[32],
                                    Core::IOMode io_pin_modes[32]) {
  using namespace omam8::Core;

  fprintf(stderr, "-----\n");
  fprintf(stderr, "PC: %#4x\tSP: %#4x\n", get_combined_register(PC),
          get_combined_register(SP));
  fprintf(stderr, "A: %#2x\t\tC:%#2x\nB: %#2x\t\tD:%#2x\t\tFlags: %d%d\n",
          get_register(A), get_register(C), get_register(B), get_register(D),
          get_flag(CMP), get_flag(CRY));
  fprintf(stderr, "IO: ");
  for (int i = 0; i < 32; i++) {
    fputc(io_pins[i] ? '1' : '0', stderr);
  }
  fprintf(stderr, "\n    ");
  for (int i = 0; i < 32; i++) {
    fputc(io_pin_modes[i] == OFF    ? 'X'
          : io_pin_modes[i] == READ ? 'R'
                                    : 'W',
          stderr);
  }
  fputc('\n', stderr);
}
