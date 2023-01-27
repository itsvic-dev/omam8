#include <core.h>
#include <helpers.h>
#include <iostream>
#include <opcodes/stack.h>


void omam8::Opcodes::pushi(uint8_t *args) {
  // pushi: 0 - immediate
  uint8_t value = args[0];
  uint16_t sp = omam8::Core::get_combined_register(omam8::Core::SP);
  omam8::Core::set_mram(sp, value);
  sp--;
  omam8::Core::set_combined_register(omam8::Core::SP, sp);
}

void omam8::Opcodes::pushr(uint8_t *args) {
  // pushr: 0 - register
  uint8_t value =
      omam8::Core::get_register(static_cast<omam8::Core::Register>(args[0]));
  uint16_t sp = omam8::Core::get_combined_register(omam8::Core::SP);
  omam8::Core::set_mram(sp, value);
  sp--;
  omam8::Core::set_combined_register(omam8::Core::SP, sp);
}

void omam8::Opcodes::popa(uint8_t *args) {
  // popa: 0 - address
  uint16_t sp = omam8::Core::get_combined_register(omam8::Core::SP);
  sp++;
  uint8_t value = omam8::Core::get_mram(sp);
  uint16_t addr = join_uint8_to_16(args[0], args[1]);
  omam8::Core::set_mram(addr, value);
  omam8::Core::set_combined_register(omam8::Core::SP, sp);
}

void omam8::Opcodes::popr(uint8_t *args) {
  // popr: 0 - register
  uint16_t sp = omam8::Core::get_combined_register(omam8::Core::SP);
  sp++;
  uint8_t value = omam8::Core::get_mram(sp);
  omam8::Core::set_register(static_cast<omam8::Core::Register>(args[0]), value);
  omam8::Core::set_combined_register(omam8::Core::SP, sp);
}
