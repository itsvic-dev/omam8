#include <core.h>
#include <opcodes/simple.h>

void omam8::Opcodes::nop(uint8_t *args) { (void)args; }

void omam8::Opcodes::hlt(uint8_t *args) {
  (void)args;
  omam8::Core::halt_cpu();
}
