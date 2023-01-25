#include "rom.h"
#include <fstream>
#include <stdexcept>

void build_and_write_rom(std::string path, std::vector<uint8_t> data) {
  std::ofstream fileStream(path, std::ios::binary | std::ios::trunc);
  if (!fileStream.is_open())
    throw std::invalid_argument("file doesn't exist");

  // write header
  fileStream.write("OMAM8ROM", 8);
  // write bank count (hardcoded)
  char banks = 1;
  fileStream.write(&banks, 1);
  // write ROM data
  fileStream.write(reinterpret_cast<char *>(&data[0]), data.size());

  fileStream.close();
}
