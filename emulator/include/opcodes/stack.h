#ifndef STACK_H
#define STACK_H

#include <cstdint>

namespace omam8 {
namespace Opcodes {
void pushi(uint8_t *args);
void pushr(uint8_t *args);
void popa(uint8_t *args);
void popr(uint8_t *args);
} // namespace Opcodes
} // namespace omam8

#endif // STACK_H
