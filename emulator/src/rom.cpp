#include <rom.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include <cstdlib>
#include <cstring>

using namespace omam8::ROM;

ROMData omam8::ROM::get_rom(std::string file) {
  std::ifstream fileStream(file, std::ios::binary);
  if (!fileStream.is_open())
    throw std::invalid_argument("file doesn't exist");

  std::vector<uint8_t> rawData;

  std::for_each(std::istreambuf_iterator<char>(fileStream),
                std::istreambuf_iterator<char>(),
                [&rawData](const char c) { rawData.push_back(c); });

  fileStream.close();

  if (rawData.size() < 0x8009)
    throw std::length_error("ROM is smaller than expected");

  if (memcmp(rawData.data(), "OMAM8ROM", 8) != 0)
    throw std::invalid_argument("magic doesn't match");

  uint8_t banks = rawData[8];
  if (banks < 1 || banks > 64)
    throw std::invalid_argument("bank amount is invalid");

  if (rawData.size() < (9 + 0x4000 + (0x4000 * banks)))
    throw std::length_error("ROM is smaller than expected by bank amount");

  uint8_t *data = rawData.data() + 9;

  return {static_cast<char>(banks), data};
}
