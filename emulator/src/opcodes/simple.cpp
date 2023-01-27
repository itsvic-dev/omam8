#include <core.h>
#include <opcodes/simple.h>

void omam8::Opcodes::nop(uint8_t *args) { (void)args; }

void omam8::Opcodes::hlt(uint8_t *args) {
  (void)args;
  omam8::Core::halt_cpu();
}

void omam8::Opcodes::movi(uint8_t *args) {
  // movi: 0 - immediate, 1 - register
  uint8_t value = args[0];
  omam8::Core::set_register(static_cast<omam8::Core::Register>(args[1]), value);
}

void omam8::Opcodes::movr(uint8_t *args) {
  // movr: 0 - register, 1 - register
  uint8_t value =
      omam8::Core::get_register(static_cast<omam8::Core::Register>(args[0]));
  omam8::Core::set_register(static_cast<omam8::Core::Register>(args[1]), value);
}
