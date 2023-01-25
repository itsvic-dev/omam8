#ifndef SHARED_H
#define SHARED_H

#include <cstdint>

enum class Opcode : uint8_t {
  NOP = 0b00000000,
  MOVI = 0b10000000,
  MOVR = 0b10000001,
  HLT = 0b11111111,
};

/**
 * Pseudo-opcodes are opcodes which expand into different opcodes.
 * For example, mov (0b1000000X) expands into movi (0b10000000) and movr
 * (0b10000001).
 */
enum class PseudoOpcode : int {
  NONE,
  MOV,
};

#endif // SHARED_H
