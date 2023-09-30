#ifndef SIMPLE_H
#define SIMPLE_H

#include <cstdint>

namespace omam8::Opcodes {

void nop(uint8_t *args);
void hlt(uint8_t *args);
void movi(uint8_t *args);
void movr(uint8_t *args);
void mov16a(uint8_t *args);
void mov16r(uint8_t *args);
void peekr(uint8_t *args);
void peeka(uint8_t *args);
void poker(uint8_t *args);
void pokea(uint8_t *args);
void clrcry(uint8_t *args);
void clrcmp(uint8_t *args);
void cryeq(uint8_t *args);
void eqi(uint8_t *args);
void eqr(uint8_t *args);
void gti(uint8_t *args);
void gtr(uint8_t *args);
void lti(uint8_t *args);
void ltr(uint8_t *args);

} // namespace omam8::Opcodes

#endif // SIMPLE_H
