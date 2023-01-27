#ifndef SIMPLE_H
#define SIMPLE_H

#include <cstdint>

namespace omam8 {
namespace Opcodes {
void nop(uint8_t *args);
void hlt(uint8_t *args);
} // namespace Opcodes
} // namespace omam8

#endif // SIMPLE_H
