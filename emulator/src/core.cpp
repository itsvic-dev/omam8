#include <core.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <map>
#include <cstdlib>
#include <cstring>

#include <opcodes/simple.h>
#include <opcodes/stack.h>
#include <opcodes/math.h>
#include <shared.h>

using namespace omam8::Core;

// ########################################
// ##         INTERNAL CORE VARS         ##
// ########################################

std::map<Opcode, EmuOpcode> opcodes {
    { Opcode::NOP, { "nop", 0, omam8::Opcodes::nop } },
    { Opcode::HLT, { "hlt", 0, omam8::Opcodes::hlt } },
    { Opcode::ADDI, { "addi", 2, omam8::Opcodes::addi } },
    { Opcode::ADDR, { "addr", 2, omam8::Opcodes::addr } },
    { Opcode::SUBI, { "subi", 2, omam8::Opcodes::subi } },
    { Opcode::SUBR, { "subr", 2, omam8::Opcodes::subr } },
    { Opcode::PUSHI, { "pushi", 1, omam8::Opcodes::pushi } },
    { Opcode::PUSHR, { "pushr", 1, omam8::Opcodes::pushr } },
    { Opcode::POPA, { "popa", 2, omam8::Opcodes::popa } },
    { Opcode::POPR, { "popr", 1, omam8::Opcodes::popr } },
};

std::map<Register, uint16_t> registers_16b {
    { Register::PC, 0x8000 },
    { Register::SP, 0x00FF },
};

std::map<Register, uint8_t> registers_8b {
    { Register::A, 0x0 },
    { Register::B, 0x0 },
    { Register::C, 0x0 },
    { Register::D, 0x0 },
};

uint8_t *memory;

omam8::ROM::ROMData rom_data;

bool cpu_running = true;

// ###############################################
// ##         PUBLIC CORE FUNCTION DEFS         ##
// ###############################################

uint8_t omam8::Core::get_register(Register reg) {
    return registers_8b[reg];
}

void omam8::Core::set_register(Register reg, uint8_t value) {
    registers_8b[reg] = value;
}

uint16_t omam8::Core::get_combined_register(unsigned int reg) {
    if (reg == Register::PC || reg == Register::SP) {
        return registers_16b[static_cast<Register>(reg)];
    }

    throw std::runtime_error("todo: get combined registers");
}

void omam8::Core::set_combined_register(unsigned int reg, uint16_t value) {
    if (reg == Register::PC || reg == Register::SP) {
        registers_16b[static_cast<Register>(reg)] = value;
        return;
    }

    throw std::runtime_error("todo: set combined registers");
}

uint8_t omam8::Core::get_mram(uint16_t addr) {
    return memory[addr];
}

void omam8::Core::set_mram(uint16_t addr, uint8_t value) {
    memory[addr] = value;
}

void omam8::Core::init() {
    memory = static_cast<uint8_t *>(malloc(0x10000));
    memset(memory, 0, 0x10000);
}

void omam8::Core::handle_opcode() {
    uint16_t pc = registers_16b[Register::PC];
    if (!opcodes.contains(static_cast<Opcode>(memory[pc]))) throw std::invalid_argument("opcode doesn't exist");
    EmuOpcode opcode = opcodes[static_cast<Opcode>(memory[pc])];
    std::cout << opcode.displayName << "\n";
    opcode.handler(memory + pc + 1u);
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

void omam8::Core::halt_cpu() {
    cpu_running = false;
}

template<typename T>
std::string int_to_hex(T i, int size) {
    std::stringstream stream;
    stream << "0x" << std::setfill('0') << std::setw(size) << std::hex << static_cast<int>(i);
    return stream.str();
}

void print_state() {
    std::cout << "PC: " << int_to_hex(get_combined_register(PC), 4) << "   ";
    std::cout << "SP: " << int_to_hex(get_combined_register(SP), 4) << "\n";
    std::cout << "A: " << int_to_hex(get_register(A), 2) << "      ";
    std::cout << "C: " << int_to_hex(get_register(C), 2) << "\n";
    std::cout << "B: " << int_to_hex(get_register(B), 2) << "      ";
    std::cout << "D: " << int_to_hex(get_register(D), 2) << "\n";
}
