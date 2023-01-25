#ifndef ROM_H
#define ROM_H

#include <cstdint>
#include <string>
#include <vector>

void build_and_write_rom(std::string path, std::vector<uint8_t> data);

#endif // ROM_H
