#ifndef SIMPLE_H
#define SIMPLE_H

#include <cstdint>

namespace omam8 {
namespace Opcodes {
void nop(uint8_t *args);
void hlt(uint8_t *args);
void movi(uint8_t *args);
void movr(uint8_t *args);
void mov16a(uint8_t *args);
void mov16r(uint8_t *args);
} // namespace Opcodes
} // namespace omam8

#endif // SIMPLE_H
