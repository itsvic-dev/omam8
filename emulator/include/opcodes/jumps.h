#ifndef JUMPS_H
#define JUMPS_H

#include <cstdint>

namespace omam8 {
namespace Opcodes {
void jmpa(uint8_t *args);
void jmpr(uint8_t *args);
} // namespace Opcodes
} // namespace omam8

#endif // JUMPS_H