#include "helpers.h"

uint16_to_8_t split_uint16_to_8(uint16_t input) {
  uint8_t lower = input & 0xFF;
  uint8_t upper = (input >> 8) & 0xFF;
  return {lower, upper};
}

uint16_t join_uint8_to_16(uint8_t lower, uint8_t upper) {
  return (upper << 8) + lower;
}
