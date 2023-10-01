#include "core.h"
#include "helpers.h"
#include <opcodes/jumps.h>

void _jmp_move_to(uint16_t addr) {
  omam8::Core::set_combined_register(omam8::Core::PC, addr);
}

void _jmp_move_by(int offset) {
  uint16_t addr = omam8::Core::get_combined_register(omam8::Core::PC);
  addr += offset;
  _jmp_move_to(addr);
}

void omam8::Opcodes::jmpa(uint8_t *args) {
  // jmpa: 0 - address
  uint16_t addr = join_uint8_to_16(args[0], args[1]);
  _jmp_move_to(addr);
}

void omam8::Opcodes::jmpr(uint8_t *args) {
  // jmpr: 0 - register
  uint16_t addr = omam8::Core::get_combined_register(args[0]);
  _jmp_move_to(addr);
}

void omam8::Opcodes::jeqa(uint8_t *args) {
  if (!Core::get_flag(Core::CMP))
    return _jmp_move_by(3);
  jmpa(args);
}

void omam8::Opcodes::jeqr(uint8_t *args) {
  if (!Core::get_flag(Core::CMP))
    return _jmp_move_by(2);
  jmpr(args);
}

void omam8::Opcodes::jnea(uint8_t *args) {
  if (Core::get_flag(Core::CMP))
    return _jmp_move_by(3);
  jmpa(args);
}

void omam8::Opcodes::jner(uint8_t *args) {
  if (Core::get_flag(Core::CMP))
    return _jmp_move_by(2);
  jmpr(args);
}
