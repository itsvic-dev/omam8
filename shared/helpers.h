#ifndef HELPERS_H
#define HELPERS_H

#include <cstdint>

typedef struct uint16_to_8 {
  uint8_t lower;
  uint8_t upper;
} uint16_to_8_t;

uint16_to_8_t split_uint16_to_8(uint16_t input);
uint16_t join_uint8_to_16(uint8_t lower, uint8_t upper);

#endif // HELPERS_H
