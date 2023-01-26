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
  PUSHI = 0b10100000,
  PUSHR = 0b10100001,
  POPR = 0b11100000,
  POPA = 0b11100001,
  SIO = 0b00010000,
  RIOIA = 0b00110000,
  RIOIR = 0b00110001,
  RIORA = 0b00110010,
  RIORR = 0b00110011,
  WIOAI = 0b00111000,
  WIOAR = 0b00111001,
  WIORI = 0b00111010,
  WIORR = 0b00111011,
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
  PUSH,
  POP,
  RIO,
  WIO,
};

#endif // SHARED_H
