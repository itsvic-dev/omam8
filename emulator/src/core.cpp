#include "helpers.h"
#include <core.h>
#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <ios>
#include <iostream>
#include <map>
#include <sstream>

#include <opcodes/io.h>
#include <opcodes/jumps.h>
#include <opcodes/math.h>
#include <opcodes/simple.h>
#include <opcodes/stack.h>
#include <shared.h>
#include <stdexcept>

using namespace omam8::Core;

// ########################################
// ##         INTERNAL CORE VARS         ##
// ########################################

std::map<Opcode, EmuOpcode> opcodes{
    {Opcode::NOP, {"nop", 0, omam8::Opcodes::nop}},
    {Opcode::HLT, {"hlt", 0, omam8::Opcodes::hlt}},
    {Opcode::MOVI, {"movi", 2, omam8::Opcodes::movi}},
    {Opcode::MOVR, {"movr", 2, omam8::Opcodes::movr}},
    {Opcode::MOV16A, {"mov16a", 3, omam8::Opcodes::mov16a}},
    {Opcode::MOV16R, {"mov16r", 3, omam8::Opcodes::mov16r}},
    {Opcode::ADDI, {"addi", 2, omam8::Opcodes::addi}},
    {Opcode::ADDR, {"addr", 2, omam8::Opcodes::addr}},
    {Opcode::SUBI, {"subi", 2, omam8::Opcodes::subi}},
    {Opcode::SUBR, {"subr", 2, omam8::Opcodes::subr}},
    {Opcode::PUSHI, {"pushi", 1, omam8::Opcodes::pushi}},
    {Opcode::PUSHR, {"pushr", 1, omam8::Opcodes::pushr}},
    {Opcode::POPA, {"popa", 2, omam8::Opcodes::popa}},
    {Opcode::POPR, {"popr", 1, omam8::Opcodes::popr}},
    {Opcode::SIOI, {"sioi", 1, omam8::Opcodes::sioi}},
    {Opcode::SIOR, {"sior", 1, omam8::Opcodes::sior}},
    {Opcode::RIOIA, {"rioia", 3, omam8::Opcodes::rioia}},
    {Opcode::RIOIR, {"rioir", 2, omam8::Opcodes::rioir}},
    {Opcode::RIORA, {"riora", 3, omam8::Opcodes::riora}},
    {Opcode::RIORR, {"riorr", 2, omam8::Opcodes::riorr}},
    {Opcode::WIOAI, {"wioai", 3, omam8::Opcodes::wioai}},
    {Opcode::WIOAR, {"wioar", 3, omam8::Opcodes::wioar}},
    {Opcode::WIORI, {"wiori", 2, omam8::Opcodes::wiori}},
    {Opcode::WIORR, {"wiorr", 2, omam8::Opcodes::wiorr}},
    {Opcode::JMPA, {"jmpa", 2, omam8::Opcodes::jmpa, true}},
    {Opcode::JMPR, {"jmpr", 1, omam8::Opcodes::jmpr, true}},
    {Opcode::SHLI, {"shli", 2, omam8::Opcodes::shli}},
    {Opcode::SHLR, {"shlr", 2, omam8::Opcodes::shlr}},
    {Opcode::SHRI, {"shri", 2, omam8::Opcodes::shri}},
    {Opcode::SHRR, {"shrr", 2, omam8::Opcodes::shrr}},
    {Opcode::PEEKR, {"peekr", 2, omam8::Opcodes::peekr}},
    {Opcode::PEEKA, {"peeka", 3, omam8::Opcodes::peeka}},
    {Opcode::POKER, {"poker", 2, omam8::Opcodes::poker}},
    {Opcode::POKEA, {"pokea", 3, omam8::Opcodes::pokea}},
};

std::map<Register, uint16_t> registers_16b{
    {Register::PC, 0x8000},
    {Register::SP, 0x00FF},
};

std::map<Register, uint8_t> registers_8b{
    {Register::A, 0x0},
    {Register::B, 0x0},
    {Register::C, 0x0},
    {Register::D, 0x0},
};

uint8_t *memory;
bool io_pins[32];
IOMode io_pin_modes[32];

omam8::ROM::ROMData rom_data;

bool cpu_running = true;

// ###############################################
// ##         PUBLIC CORE FUNCTION DEFS         ##
// ###############################################

uint8_t omam8::Core::get_register(Register reg) { return registers_8b[reg]; }

void omam8::Core::set_register(Register reg, uint8_t value) {
  registers_8b[reg] = value;
}

uint16_t omam8::Core::get_combined_register(unsigned int reg) {
  if (reg == Register::PC || reg == Register::SP) {
    return registers_16b[static_cast<Register>(reg)];
  }

  if (reg == (Register::B | Register::A)) {
    return join_uint8_to_16(registers_8b[B], registers_8b[A]);
  }

  if (reg == (Register::D | Register::C)) {
    return join_uint8_to_16(registers_8b[D], registers_8b[C]);
  }

  throw std::logic_error("invalid combined register");
}

void omam8::Core::set_combined_register(unsigned int reg, uint16_t value) {
  if (reg == Register::PC || reg == Register::SP) {
    registers_16b[static_cast<Register>(reg)] = value;
    return;
  }

  uint16_to_8_t value_8b = split_uint16_to_8(value);

  if (reg == (Register::B | Register::A)) {
    registers_8b[B] = value_8b.lower;
    registers_8b[A] = value_8b.upper;
    return;
  }

  if (reg == (Register::D | Register::C)) {
    registers_8b[D] = value_8b.lower;
    registers_8b[C] = value_8b.upper;
    return;
  }

  throw std::logic_error("invalid combined register");
}

uint8_t omam8::Core::get_mram(uint16_t addr) { return memory[addr]; }

void omam8::Core::set_mram(uint16_t addr, uint8_t value) {
  if (addr >= 0x8000 && addr < 0xC000) {
    std::cerr << "ignoring write to RAM address " << std::hex << addr
              << std::endl;
    return;
  }
  if (addr >= 0xC000) {
    // bank switch
    if (value >= rom_data.banks) {
      memset(memory + 0xC000, 0xFF, 0xFFFF - 0xC000);
    } else {
      throw std::logic_error("switching banks isn't fully implemented yet!!!");
    }
    return;
  }
  memory[addr] = value;
}

bool omam8::Core::read_io_pin(int io_pin) {
  if (io_pin > 31)
    throw std::logic_error("cannot read from non-existent pin");
  if (io_pin_modes[io_pin] != READ) {
    throw std::logic_error("cannot read from a pin that isn't in read mode");
  }
  return io_pins[io_pin];
}

void omam8::Core::write_io_pin(int io_pin, bool value) {
  if (io_pin > 31)
    throw std::logic_error("cannot write to non-existent pin");
  if (io_pin_modes[io_pin] != WRITE) {
    throw std::logic_error("cannot write to a pin that isn't in write mode");
  }
  io_pins[io_pin] = value;
}

void omam8::Core::set_io_pin_mode(int io_pin, IOMode mode) {
  if (io_pin > 31)
    throw std::logic_error("cannot set mode of non-existent pin");
  io_pin_modes[io_pin] = mode;
  // we clear the pin on state change
  io_pins[io_pin] = false;
}

void omam8::Core::init() {
  memory = static_cast<uint8_t *>(malloc(0x10000));
  memset(memory, 0, 0x10000);
  memset(io_pins, 0, sizeof(io_pins));
  memset(io_pin_modes, OFF, sizeof(io_pin_modes));
}

void omam8::Core::handle_opcode() {
  uint16_t pc = registers_16b[Register::PC];
  if (!opcodes.contains(static_cast<Opcode>(memory[pc])))
    throw std::invalid_argument("opcode doesn't exist");
  EmuOpcode opcode = opcodes[static_cast<Opcode>(memory[pc])];
  std::cout << opcode.displayName << "\n";
  opcode.handler(memory + pc + 1u);
  if (!opcode.manipulatesPC)
    registers_16b[Register::PC] = pc + 1u + opcode.argsLength;
}

void print_state();

void omam8::Core::start_loop() {
  print_state();
  while (cpu_running) {
    handle_opcode();
    print_state();
  }
}

void omam8::Core::load_rom(omam8::ROM::ROMData rom) {
  rom_data = rom;
  memcpy(&memory[0x8000], rom.data, 0x8000);
}

void omam8::Core::halt_cpu() { cpu_running = false; }

template <typename T> std::string int_to_hex(T i, int size) {
  std::stringstream stream;
  stream << "0x" << std::setfill('0') << std::setw(size) << std::hex
         << static_cast<int>(i);
  return stream.str();
}

void print_state() {
  std::cout << "PC: " << int_to_hex(get_combined_register(PC), 4) << "   ";
  std::cout << "SP: " << int_to_hex(get_combined_register(SP), 4) << "\n";
  std::cout << "A: " << int_to_hex(get_register(A), 2) << "      ";
  std::cout << "C: " << int_to_hex(get_register(C), 2) << "\n";
  std::cout << "B: " << int_to_hex(get_register(B), 2) << "      ";
  std::cout << "D: " << int_to_hex(get_register(D), 2) << "\n";
  std::cout << "IO: ";
  for (int i = 0; i < 32; i++) {
    std::cout << (io_pins[i] ? "1" : "0");
  }
  std::cout << "\n";
  std::cout << "    ";
  for (int i = 0; i < 32; i++) {
    std::cout << (io_pin_modes[i] == OFF    ? "X"
                  : io_pin_modes[i] == READ ? "R"
                                            : "W");
  }
  std::cout << "\n";
}
