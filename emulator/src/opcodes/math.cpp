#include <core.h>
#include <opcodes/math.h>

void omam8::Opcodes::addi(uint8_t *args) {
  // addi: 0 - immediate, 1 - register
  uint8_t a = args[0];
  omam8::Core::Register reg = static_cast<omam8::Core::Register>(args[1]);
  uint8_t b = omam8::Core::get_register(reg);
  omam8::Core::set_register(reg, a + b);
}

void omam8::Opcodes::addr(uint8_t *args) {
  // addr: 0 - register, 1 - register
  omam8::Core::Register reg1 = static_cast<omam8::Core::Register>(args[0]);
  omam8::Core::Register reg2 = static_cast<omam8::Core::Register>(args[1]);

  uint8_t a = omam8::Core::get_register(reg1);
  uint8_t b = omam8::Core::get_register(reg2);
  omam8::Core::set_register(reg2, a + b);
}

void omam8::Opcodes::subi(uint8_t *args) {
  // subi: 0 - immediate, 1 - register
  uint8_t a = args[0];
  omam8::Core::Register reg = static_cast<omam8::Core::Register>(args[1]);
  uint8_t b = omam8::Core::get_register(reg);
  omam8::Core::set_register(reg, b - a);
}

void omam8::Opcodes::subr(uint8_t *args) {
  // subr: 0 - register, 1 - register
  omam8::Core::Register reg1 = static_cast<omam8::Core::Register>(args[0]);
  omam8::Core::Register reg2 = static_cast<omam8::Core::Register>(args[1]);

  uint8_t a = omam8::Core::get_register(reg1);
  uint8_t b = omam8::Core::get_register(reg2);
  omam8::Core::set_register(reg2, b - a);
}

void omam8::Opcodes::shli(uint8_t *args) {
  // shli: 0 - immediate, 1 - register
  uint8_t amount = args[0];
  omam8::Core::Register reg = static_cast<omam8::Core::Register>(args[1]);

  uint8_t value = omam8::Core::get_register(reg) << amount;
  omam8::Core::set_register(reg, value);
}

void omam8::Opcodes::shlr(uint8_t *args) {
  // shlr: 0 - register, 1 - register
  omam8::Core::Register reg1 = static_cast<omam8::Core::Register>(args[0]);
  omam8::Core::Register reg2 = static_cast<omam8::Core::Register>(args[1]);

  int amount = omam8::Core::get_register(reg1);
  uint8_t value = omam8::Core::get_register(reg2) << amount;
  omam8::Core::set_register(reg2, value);
}

void omam8::Opcodes::shri(uint8_t *args) {
  // shri: 0 - immediate, 1 - register
  uint8_t amount = args[0];
  omam8::Core::Register reg = static_cast<omam8::Core::Register>(args[1]);

  uint8_t value = omam8::Core::get_register(reg) >> amount;
  omam8::Core::set_register(reg, value);
}

void omam8::Opcodes::shrr(uint8_t *args) {
  // shrr: 0 - register, 1 - register
  omam8::Core::Register reg1 = static_cast<omam8::Core::Register>(args[0]);
  omam8::Core::Register reg2 = static_cast<omam8::Core::Register>(args[1]);

  int amount = omam8::Core::get_register(reg1);
  uint8_t value = omam8::Core::get_register(reg2) >> amount;
  omam8::Core::set_register(reg2, value);
}
