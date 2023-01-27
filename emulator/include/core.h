#ifndef CORE_H
#define CORE_H

#include <cstdint>
#include <string>

#include <rom.h>

namespace omam8 {
namespace Core {
typedef struct {
  std::string displayName;
  unsigned int argsLength;
  void (*handler)(uint8_t *args);
} EmuOpcode;

enum Register : unsigned int {
  PC = 0b010000,
  SP = 0b100000,
  A = 0b000001,
  B = 0b000010,
  C = 0b000100,
  D = 0b001000,
};

uint8_t get_register(Register reg);
void set_register(Register reg, uint8_t value);
uint16_t get_combined_register(unsigned int reg);
void set_combined_register(unsigned int reg, uint16_t value);

uint8_t get_mram(uint16_t addr);
void set_mram(uint16_t addr, uint8_t value);

void init();
void handle_opcode();
void start_loop();

void load_rom(omam8::ROM::ROMData rom);

void halt_cpu();
}; // namespace Core
}; // namespace omam8

#endif // CORE_H
