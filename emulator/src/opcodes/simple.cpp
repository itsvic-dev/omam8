#include <core.h>
#include <opcodes/simple.h>
#include <helpers.h>

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

void omam8::Opcodes::mov16a(uint8_t *args) {
  // mov16a: 0 - address, 2 - combined register
  uint16_t address = join_uint8_to_16(args[0], args[1]);
  uint8_t reg = args[2];
  omam8::Core::set_combined_register(reg, address);
}

void omam8::Opcodes::mov16r(uint8_t *args) {
  // mov16r: 0 - combined register, 1 - combined register
  uint8_t reg1 = args[0];
  uint8_t reg2 = args[1];
  uint16_t value = omam8::Core::get_combined_register(reg1);
  omam8::Core::set_combined_register(reg2, value);
}
