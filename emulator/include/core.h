#ifndef CORE_H
#define CORE_H

#include <cstdint>
#include <string>

#include <rom.h>

namespace omam8::Core {

typedef struct EmuOpcode {
  std::string displayName;
  unsigned int argsLength;
  void (*handler)(uint8_t *args);
  bool manipulatesPC = false;
} EmuOpcode;

enum Register : unsigned int {
  PC = 0b00010000,
  SP = 0b00100000,
  A = 0b00000001,
  B = 0b00000010,
  C = 0b00000100,
  D = 0b00001000,
  CMP = 0b01000000,
  CRY = 0b10000000,
};

enum IOMode : uint8_t {
  OFF = 0b00,
  READ = 0b01,
  WRITE = 0b10,
};

uint8_t get_register(Register reg);
void set_register(Register reg, uint8_t value);
uint16_t get_combined_register(unsigned int reg);
void set_combined_register(unsigned int reg, uint16_t value);

uint8_t get_mram(uint16_t addr);
void set_mram(uint16_t addr, uint8_t value);

bool read_io_pin(int io_pin);
void write_io_pin(int io_pin, bool value);
void set_io_pin_mode(int io_pin, IOMode mode);

void init();
void handle_opcode();
void start_loop();

void load_rom(omam8::ROM::ROMData rom);

void halt_cpu();

}; // namespace omam8::Core

#endif // CORE_H
