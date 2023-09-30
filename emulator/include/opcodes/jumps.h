#ifndef JUMPS_H
#define JUMPS_H

#include <cstdint>

namespace omam8::Opcodes {

void jmpa(uint8_t *args);
void jmpr(uint8_t *args);

} // namespace omam8::Opcodes

#endif // JUMPS_H