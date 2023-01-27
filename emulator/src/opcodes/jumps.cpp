#include "core.h"
#include "helpers.h"
#include <opcodes/jumps.h>

void omam8::Opcodes::jmpa(uint8_t *args) {
  // jmpa: 0 - address
  uint16_t addr = join_uint8_to_16(args[0], args[1]);
  omam8::Core::set_combined_register(omam8::Core::PC, addr);
}

void omam8::Opcodes::jmpr(uint8_t *args) {
  // jmpr: 0 - register
  uint16_t addr = omam8::Core::get_combined_register(args[0]);
  omam8::Core::set_combined_register(omam8::Core::PC, addr);
}
