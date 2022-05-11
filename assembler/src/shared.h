#ifndef SHARED_H
#define SHARED_H

#include <cstdint>

enum class Opcode : uint8_t {
    NOP = 0x00,
    MOV = 0x01,
    MOV16 = 0x02,
    LOAD = 0x03,
    STOR = 0x04,
    EQ = 0x11,
    NE = 0x12,
    LT = 0x13,
    GT = 0x14,
    LE = 0x15,
    GE = 0x16,
    JMP = 0x20,
    JEQ = 0x21,
    JNE = 0x22,
    JLT = 0x23,
    JGT = 0x24,
    JLE = 0x25,
    JGE = 0x26,
    SIO = 0x30,
    RIO = 0x31,
    WIO = 0x32,
    ADD = 0x40,
    SUB = 0x41,
    SHL = 0x42,
    SHR = 0x43,
    HLT = 0xFF,
};

#endif // SHARED_H
