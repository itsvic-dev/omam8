#include <core.h>
#include <iostream>
#include <map>

#include <cstdlib>
#include <cstring>

#include <opcodes/simple.h>

using namespace omam8::Core;

// ########################################
// ##         INTERNAL CORE VARS         ##
// ########################################

std::map<uint8_t, Opcode> opcodes {
    { 0x00, { "nop", 0, omam8::Opcodes::nop } },
    { 0xFF, { "hlt", 0, omam8::Opcodes::hlt } }
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

uint16_t omam8::Core::get_combined_register(unsigned int reg) {
    if (reg == Register::PC || reg == Register::SP) {
        return registers_16b[static_cast<Register>(reg)];
    }

    return 0xFF; // TODO: combined registers
}

void omam8::Core::init() {
    memory = static_cast<uint8_t *>(malloc(0x10000));
    memset(memory, 0, 0x10000);
}

void omam8::Core::handle_opcode() {
    uint16_t pc = registers_16b[Register::PC];
    if (!opcodes.contains(memory[pc])) throw std::invalid_argument("opcode doesn't exist");
    Opcode opcode = opcodes[memory[pc]];
    std::cout << opcode.displayName << "\n";
    opcode.handler(memory + pc + 1u);
    registers_16b[Register::PC] = pc + 1u + opcode.argsLength;
}

void omam8::Core::start_loop() {
    while (cpu_running) {
        handle_opcode();
    }
}

void omam8::Core::load_rom(omam8::ROM::ROMData rom) {
    rom_data = rom;
    memcpy(&memory[0x8000], rom.data, 0x8000);
}

void omam8::Core::halt_cpu() {
    cpu_running = false;
}
