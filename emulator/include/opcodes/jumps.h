#ifndef JUMPS_H
#define JUMPS_H

#include <cstdint>

namespace omam8::Opcodes {

void jmpa(uint8_t *args);
void jmpr(uint8_t *args);
void jeqa(uint8_t *args);
void jeqr(uint8_t *args);
void jnea(uint8_t *args);
void jner(uint8_t *args);

} // namespace omam8::Opcodes

#endif // JUMPS_H
