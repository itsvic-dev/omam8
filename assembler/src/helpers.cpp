#include "helpers.h"

uint16_to_8_t split_uint16_to_8(uint16_t input) {
  uint8_t lower = input & 0xFF;
  uint8_t upper = (input >> 8) & 0xFF;
  return {lower, upper};
}
