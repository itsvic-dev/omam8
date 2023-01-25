#ifndef SHARED_H
#define SHARED_H

#include <cstdint>

enum class Opcode : uint8_t {
  NOP = 0b00000000,
  HLT = 0b11111111,
  MOVI = 0b10000000,
  MOVR = 0b10000001,
  ADDI = 0b01000000,
  ADDR = 0b01000001,
  SUBI = 0b01100000,
  SUBR = 0b01100001,
};

/**
 * Pseudo-opcodes are opcodes which expand into different opcodes.
 * For example, mov (0b1000000X) expands into movi (0b10000000) and movr
 * (0b10000001).
 */
enum class PseudoOpcode : int {
  NONE,
  MOV,
  ADD,
  SUB,
};

#endif // SHARED_H
