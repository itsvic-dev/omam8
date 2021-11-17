#include <core.h>
#include <iostream>
#include <cstring>

void EmulatorCore::initialize(std::vector<uint8_t> rom, bool verbose) {
    // set the memory to 0s
    memset(mram, 0, 0xFFFF);
    memset(vram, 0, 0x9600);

    // fill the ROM region of MRAM
    for (int i = 0x8000; i <= 0xFFFF; i++) {
        if (i - 0x8000 > rom.size()) break;
        mram[i] = rom[i - 0x8000];
    }

    initialized = true;
    this->verbose = verbose;
    std::cout << "[EMULATOR CORE] Initialized." << std::endl;
}

void EmulatorCore::initialize(std::vector<uint8_t> rom) {
    initialize(rom, false);
}

uint16_t get_16bit_from_8bit(uint8_t lower, uint8_t upper) {
    return ((uint16_t) upper << 8) | lower;
}

void EmulatorCore::run_clock_cycle() {
    if (!initialized) {
        std::cerr << "[EMULATOR CORE] Trying to cycle while not initialized. Did you forget to run ::initialize?" << std::endl;
        return;
    }

    uint8_t instruction = mram[reg_pc];
    switch(instruction) {
        case 0x00: { // hlt
            if (verbose) printf("hlt\n");
            throw 0xFF;
        }
        case 0x01: { // lda
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_a = mram[addr];
            reg_pc += 3;
            if (verbose) printf("lda 0x%04X\n", addr);
            return;
        }
        case 0x02: { // ldb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_b = mram[addr];
            reg_pc += 3;
            if (verbose) printf("ldb 0x%04X\n", addr);
            return;
        }
        case 0x03: { // sta
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            mram[addr] = reg_a;
            reg_pc += 3;
            if (verbose) printf("sta 0x%04X\n", addr);
            return;
        }
        case 0x04: { // stb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            mram[addr] = reg_b;
            reg_pc += 3;
            if (verbose) printf("stb 0x%04X\n", addr);
            return;
        }
        case 0x05: { // adda
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_a += mram[addr];
            reg_pc += 3;
            if (verbose) printf("adda 0x%04X\n", addr);
            return;
        }
        case 0x06: { // addb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_b += mram[addr];
            reg_pc += 3;
            if (verbose) printf("addb 0x%04X\n", addr);
            return;
        }
        case 0x07: { // suba
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_a -= mram[addr];
            reg_pc += 3;
            if (verbose) printf("suba 0x%04X\n", addr);
            return;
        }
        case 0x08: { // subb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_b -= mram[addr];
            reg_pc += 3;
            if (verbose) printf("subb 0x%04X\n", addr);
            return;
        }
        case 0x09: { // j
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_pc = addr;
            if (verbose) printf("j 0x%04X\n", addr);
            return;
        }
        case 0x0A: { // jab
            uint16_t addr = get_16bit_from_8bit(reg_a, reg_b);
            reg_pc = addr;
            if (verbose) printf("jab\n");
            return;
        }
        case 0x0B: { // spua
            mram[reg_sp] = reg_a;
            reg_sp--;
            reg_pc += 1;
            if (verbose) printf("spua\n");
            return;
        }
        case 0x0C: { // spub
            mram[reg_sp] = reg_b;
            reg_sp--;
            reg_pc += 1;
            if (verbose) printf("spub\n");
            return;
        }
        case 0x0D: { // spoa
            reg_sp++;
            reg_a = mram[reg_sp];
            reg_pc += 1;
            if (verbose) printf("spoa\n");
            return;
        }
        case 0x0E: { // spob
            reg_sp++;
            reg_b = mram[reg_sp];
            reg_pc += 1;
            if (verbose) printf("spob\n");
            return;
        }
        case 0x0F: { // vlda
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (addr >= 0x9600) {
                fprintf(stderr, "[EMULATOR CORE] vlda: attempted read beyond VRAM region (0x%04X), halting the CPU.\n", addr);
                throw 0xFF;
            }
            reg_a = vram[addr];
            reg_pc += 3;
            if (verbose) printf("vlda 0x%04X\n", addr);
            return;
        }
        case 0x10: { // vldb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (addr >= 0x9600) {
                fprintf(stderr, "[EMULATOR CORE] vldb: attempted read beyond VRAM region (0x%04X), halting the CPU.\n", addr);
                throw 0xFF;
            }
            reg_b = vram[addr];
            reg_pc += 3;
            if (verbose) printf("vldb 0x%04X\n", addr);
            return;
        }
        case 0x11: { // vsta
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (addr >= 0x9600) {
                fprintf(stderr, "[EMULATOR CORE] vsta: attempted write beyond VRAM region (0x%04X), halting the CPU.\n", addr);
                throw 0xFF;
            }
            vram[addr] = reg_a;
            reg_pc += 3;
            if (verbose) printf("vsta 0x%04X\n", addr);
            return;
        }
        case 0x12: { // vstb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (addr >= 0x9600) {
                fprintf(stderr, "[EMULATOR CORE] vstb: attempted write beyond VRAM region (0x%04X), halting the CPU.\n", addr);
                throw 0xFF;
            }
            vram[addr] = reg_b;
            reg_pc += 3;
            if (verbose) printf("vstb 0x%04X\n", addr);
            return;
        }
        case 0x13:   // inab
        case 0x14:   // outab
        case 0x15: { // pinab
            std::cerr << "[EMULATOR CORE] pin instruction called, not supported." << std::endl;
            throw 1;
        }
        case 0x16: { // jza
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (reg_a == 0) {
                reg_pc = addr;
            } else {
                reg_pc += 3;
            }
            if (verbose) printf("jza 0x%04X\n", addr);
            return;
        }
        case 0x17: { // jzb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (reg_b == 0) {
                reg_pc = addr;
            } else {
                reg_pc += 3;
            }
            if (verbose) printf("jzb 0x%04X\n", addr);
            return;
        }
        case 0x18: { // jnza
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (reg_a != 0) {
                reg_pc = addr;
            } else {
                reg_pc += 3;
            }
            if (verbose) printf("jnza 0x%04X\n", addr);
            return;
        }
        case 0x19: { // jnzb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (reg_b != 0) {
                reg_pc = addr;
            } else {
                reg_pc += 3;
            }
            if (verbose) printf("jnzb 0x%04X\n", addr);
            return;
        }
        case 0x1A: { // jeqab
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (reg_a == reg_b) {
                reg_pc = addr;
            } else {
                reg_pc += 3;
            }
            if (verbose) printf("jeqab 0x%04X\n", addr);
            return;
        }
        case 0x1B:   // tsta
        case 0x1C:   // tsto
        case 0x1D:   // tldal
        case 0x1E:   // tldau
        case 0x1F:   // tldbl
        case 0x20: { // tldbu
            std::cerr << "[EMULATOR CORE] timer instruction called, not supported." << std::endl;
            throw 1;
        }
        case 0x21: { // ldra
            reg_a = reg_r;
            reg_pc += 1;
            if (verbose) printf("ldra\n");
            return;
        }
        case 0x22: { // ldrb
            reg_b = reg_r;
            reg_pc += 1;
            if (verbose) printf("ldra\n");
            return;
        }
        case 0x23: { // jnzr
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (reg_r != 0) {
                reg_pc = addr;
            } else {
                reg_pc += 3;
            }
            if (verbose) printf("jnzr 0x%04X\n", addr);
            return;
        }
        case 0x24: { // lsha
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_a = reg_a << mram[addr];
            reg_pc += 3;
            if (verbose) printf("lsha 0x%04X\n", addr);
            return;
        }
        case 0x25: { // lshb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_b = reg_b << mram[addr];
            reg_pc += 3;
            if (verbose) printf("lshb 0x%04X\n", addr);
            return;
        }
        case 0x26: { // lshab
            reg_a = reg_a << reg_b;
            reg_pc += 1;
            if (verbose) printf("lshab\n");
            return;
        }
        case 0x27: { // rsha
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_a = reg_a >> mram[addr];
            reg_pc += 3;
            if (verbose) printf("rsha 0x%04X\n", addr);
            return;
        }
        case 0x28: { // rshb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_b = reg_b >> mram[addr];
            reg_pc += 3;
            if (verbose) printf("rshb 0x%04X\n", addr);
            return;
        }
        case 0x29: { // rshab
            reg_a = reg_a >> reg_b;
            reg_pc += 1;
            if (verbose) printf("rshab\n");
            return;
        }
        case 0x2A: { // anda
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_a = reg_a & mram[addr];
            reg_pc += 3;
            if (verbose) printf("anda 0x%04X\n", addr);
            return;
        }
        case 0x2B: { // andb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_b = reg_b & mram[addr];
            reg_pc += 3;
            if (verbose) printf("andb 0x%04X\n", addr);
            return;
        }
        case 0x2C: { // andab
            reg_a = reg_a & reg_b;
            reg_pc += 1;
            if (verbose) printf("andab\n");
            return;
        }
        case 0x2D: { // ora
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_a = reg_a | mram[addr];
            reg_pc += 3;
            if (verbose) printf("ora 0x%04X\n", addr);
            return;
        }
        case 0x2E: { // orb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_b = reg_b | mram[addr];
            reg_pc += 3;
            if (verbose) printf("orb 0x%04X\n", addr);
            return;
        }
        case 0x2F: { // orab
            reg_a = reg_a | reg_b;
            reg_pc += 1;
            if (verbose) printf("orab\n");
            return;
        }
        case 0x30: { // xora
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_a = reg_a ^ mram[addr];
            reg_pc += 3;
            if (verbose) printf("xora 0x%04X\n", addr);
            return;
        }
        case 0x31: { // xorb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_b = reg_b ^ mram[addr];
            reg_pc += 3;
            if (verbose) printf("xorb 0x%04X\n", addr);
            return;
        }
        case 0x32: { // xorab
            reg_a = reg_a ^ reg_b;
            reg_pc += 1;
            if (verbose) printf("xorab\n");
            return;
        }
        case 0x33: { // jz
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (reg_zf == 1) {
                reg_pc = addr;
            } else {
                reg_pc += 3;
            }
            if (verbose) printf("jz 0x%04X\n", addr);
            return;
        }
        case 0x34: { // jnz
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            if (reg_zf == 0) {
                reg_pc = addr;
            } else {
                reg_pc += 3;
            }
            if (verbose) printf("jnz 0x%04X\n", addr);
            return;
        }
        case 0x35: { // setea
            reg_a = reg_zf;
            reg_pc += 1;
            if (verbose) printf("setea\n");
            return;
        }
        case 0x36: { // seteb
            reg_b = reg_zf;
            reg_pc += 1;
            if (verbose) printf("seteb\n");
            return;
        }
        case 0x37: { // setnea
            reg_a = !reg_zf;
            reg_pc += 1;
            if (verbose) printf("setnea\n");
            return;
        }
        case 0x38: { // setneb
            reg_b = !reg_zf;
            reg_pc += 1;
            if (verbose) printf("setneb\n");
            return;
        }
        case 0x39: { // cmpa
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_zf = reg_a == mram[addr];
            reg_pc += 3;
            if (verbose) printf("cmpa 0x%04X\n", addr);
            return;
        }
        case 0x3A: { // cmpb
            uint16_t addr = get_16bit_from_8bit(mram[reg_pc + 1], mram[reg_pc + 2]);
            reg_zf = reg_b == mram[addr];
            reg_pc += 3;
            if (verbose) printf("cmpb 0x%04X\n", addr);
            return;
        }
        case 0x3B: { // cmpab
            reg_zf = reg_a == reg_b;
            reg_pc += 1;
            if (verbose) printf("cmpab\n");
            return;
        }
        case 0x3C: { // addab
            reg_a += reg_b;
            reg_pc += 1;
            if (verbose) printf("addab\n");
            return;
        }
        case 0x3D: { // subab
            reg_a -= reg_b;
            reg_pc += 1;
            if (verbose) printf("subab\n");
            return;
        }
        default:
            std::cout << "[EMULATOR CORE] Unknown instruction ";
            printf("0x%02X", instruction);
            std::cout << "." << std::endl;
            throw 1;
    }
}
