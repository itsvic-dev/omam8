#ifndef IO_H
#define IO_H

#include <cstdint>

namespace omam8::Opcodes {

void sioi(uint8_t *args);
void sior(uint8_t *args);
void rioia(uint8_t *args);
void rioir(uint8_t *args);
void riora(uint8_t *args);
void riorr(uint8_t *args);
void wioai(uint8_t *args);
void wioar(uint8_t *args);
void wiori(uint8_t *args);
void wiorr(uint8_t *args);

} // namespace omam8::Opcodes

#endif // IO_H
