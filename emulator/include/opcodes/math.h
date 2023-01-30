#ifndef MATH_H
#define MATH_H

#include <cstdint>

namespace omam8 {
namespace Opcodes {
void addi(uint8_t *args);
void addr(uint8_t *args);
void subi(uint8_t *args);
void subr(uint8_t *args);
void shli(uint8_t *args);
void shlr(uint8_t *args);
void shri(uint8_t *args);
void shrr(uint8_t *args);
} // namespace Opcodes
} // namespace omam8

#endif // MATH_H
