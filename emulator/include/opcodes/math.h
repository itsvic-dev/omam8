#ifndef MATH_H
#define MATH_H

#include <cstdint>

namespace omam8 {
namespace Opcodes {
void addi(uint8_t *args);
void addr(uint8_t *args);
void subi(uint8_t *args);
void subr(uint8_t *args);
void andi(uint8_t *args);
void andr(uint8_t *args);
void ori(uint8_t *args);
void orr(uint8_t *args);
void xori(uint8_t *args);
void xorr(uint8_t *args);
void _not(uint8_t *args);
void shli(uint8_t *args);
void shlr(uint8_t *args);
void shri(uint8_t *args);
void shrr(uint8_t *args);
} // namespace Opcodes
} // namespace omam8

#endif // MATH_H
