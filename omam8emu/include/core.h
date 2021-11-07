#ifndef CORE_H
#define CORE_H
#include <cstdint>
#include <vector>

class EmulatorCore {
public:
    void initialize(std::vector<uint8_t> rom);
    void initialize(std::vector<uint8_t> rom, bool verbose);
    void run_clock_cycle();

    uint8_t reg_a = 0;
    uint8_t reg_b = 0;
    uint8_t reg_r = 0; // remainder
    uint8_t reg_zf = 0; // zero flag
    uint16_t reg_pc = 0x8000; // program counter
    uint16_t reg_sp = 0x00FF; // stack pointer

    uint8_t mram[0xFFFF];
    uint8_t vram[0x9600];

private:
    bool initialized = false;
    bool verbose = false;
};

#endif // CORE_H
