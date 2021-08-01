#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

uint8_t mram[0xFFFF];
uint8_t vram[0x9600];
uint16_t program_counter = 0x8000;
uint8_t register_remainder;
uint16_t stack_pointer;
uint8_t register_a;
uint16_t register_time;
uint8_t pins[10];
bool pin_is_output[10];

time_t internal_timer_start;
time_t internal_timer_stop;

bool cpu_active = true;

// ----------------------------------------------------------------
// INSTRUCTIONS
// ----------------------------------------------------------------

void jmp(uint16_t addr) {
    program_counter = addr;
}

void jnz(uint16_t addr) {
    if (register_remainder != 0) {
        program_counter = addr;
    } else {
        program_counter += 3;
    }
}

void jz(uint16_t addr) {
    if (register_remainder == 0) {
        program_counter = addr;
    } else {
        program_counter += 3;
    }
}

void add(uint16_t x, uint16_t y) {
    uint16_t tmp = (uint16_t) mram[x] + (uint16_t) mram[y];
    if (tmp > 0xFF) {
        register_remainder = (uint8_t) (tmp >> 0x8 & 0xFF);
        tmp = tmp & 0xFF;
    }
    mram[y] = (uint8_t) tmp;
}

void sub(uint16_t x, uint16_t y) {
    mram[y] = mram[x] - mram[y];
}

void mov(uint16_t x, uint16_t y) {
    if (y > sizeof(mram)) {
        printf("MOV tried to write beyond MRAM region, halting.\n");
        cpu_active = false;
    }
    mram[y] = mram[x];
}

void vmov(uint16_t x, uint16_t y) {
    if (y > sizeof(vram)) {
        printf("VMOV tried to write beyond VRAM region, halting.\n");
        cpu_active = false;
    }
    vram[y] = mram[x];
}

void mul(uint16_t x, uint16_t y) {
    mram[y] = mram[x] * mram[y];
}

void _div(uint16_t x, uint16_t y) {
    mram[y] = mram[x] / mram[y];
}

void spu(uint16_t addr) {
    mram[stack_pointer] = mram[addr];
    stack_pointer ++;
}

void spo(uint16_t addr) {
    stack_pointer --;
    mram[addr] = mram[stack_pointer];
}

void clr(void) {
    register_remainder = 0;
}

void and(uint16_t x, uint16_t y) {
    mram[y] = mram[x] & mram[y];
}

void vrd(uint16_t x, uint16_t y) {
    if (x > sizeof(vram)) {
        printf("VRD tried to read beyond VRAM region, halting.\n");
        cpu_active = false;
    }
    mram[y] = vram[x];
}

void lda(uint16_t x) {
    if (x > sizeof(mram)) {
        printf("LDA tried to read beyond MRAM region, halting.\n");
        cpu_active = false;
    }
    register_a = mram[x];
}

void sta(uint16_t x) {
    if (x > sizeof(mram)) {
        printf("STA tried to write beyond MRAM region, halting.\n");
        cpu_active = false;
    }
    mram[x] = register_a;
}

void jeq(uint16_t x, uint16_t y) {
    if (register_a == mram[x]) {
        program_counter = y;
    } else {
        program_counter += 5;
    }
}

void spin(uint16_t x, uint16_t y) {
    pin_is_output[mram[x]] = mram[y];
    pins[mram[x]] = 0;
}

void in(uint16_t x, uint16_t y) {
    if (pin_is_output[mram[x]]) {
        mram[y] = 0xFF;
        return;
    }
    mram[y] = pins[mram[x]];
}

void out(uint16_t x, uint16_t y) {
    if (!pin_is_output[mram[x]]) {
        return;
    }
    pins[mram[x]] = mram[y];
}

void tsta(void) {
    internal_timer_start = time(NULL) * 1000;
}

void tsto(void) {
    internal_timer_stop = time(NULL) * 1000;
    
    register_time = (uint16_t) (internal_timer_stop - internal_timer_start);
}

void stt(uint16_t addr) {
    uint8_t lower = (uint8_t) (register_time & 0xFF);
    uint8_t upper = (uint8_t) (register_time * 0x100 & 0xFF);
    mram[addr] = lower;
    mram[addr + 1] = upper;
}

void lsh(uint16_t x, uint16_t y) {
    mram[y] = mram[x] << mram[y];
}

void rsh(uint16_t x, uint16_t y) {
    mram[y] = mram[x] >> mram[y];
}

void hlt(void) {
    cpu_active = false;
}

// ----------------------------------------------------------------
// OTHER LOGIC
// ----------------------------------------------------------------

uint16_t get_16bit_from_mram(uint16_t addr) {
    uint8_t lower = mram[addr];
    uint8_t upper = mram[addr + 1];
    
    return (upper * 0x100) + lower;
}

void execute(void) {
    uint8_t instruction = mram[program_counter];

    switch (instruction) {
        case 0x00:
            jmp(get_16bit_from_mram(program_counter + 1));
            return;
        
        case 0x01:
            jnz(get_16bit_from_mram(program_counter + 1));
            return;
        
        case 0x02:
            jz(get_16bit_from_mram(program_counter + 1));
            return;
        
        case 0x03:
            add(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x04:
            sub(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x05:
            mov(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x06:
            vmov(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x07:
            mul(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x08:
            _div(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x09:
            spu(get_16bit_from_mram(program_counter + 1));
            program_counter += 3;
            return;
        
        case 0x0A:
            spo(get_16bit_from_mram(program_counter + 1));
            program_counter += 3;
            return;
        
        case 0x0B:
            clr();
            program_counter += 1;
            return;
        
        case 0x0C:
            and(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x0D:
            vrd(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x0E:
            lda(get_16bit_from_mram(program_counter + 1));
            program_counter += 3;
            return;
        
        case 0x0F:
            sta(get_16bit_from_mram(program_counter + 1));
            program_counter += 3;
            return;
        
        case 0x10:
            jeq(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            return;

        case 0x11:
            spin(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;

        case 0x12:
            in(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x13:
            out(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x14:
            tsta();
            program_counter += 1;
            return;
        
        case 0x15:
            tsto();
            program_counter += 1;
            return;
        
        case 0x16:
            stt(get_16bit_from_mram(program_counter + 1));
            program_counter += 3;
            return;
        
        case 0x17:
            lsh(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;
        
        case 0x18:
            rsh(get_16bit_from_mram(program_counter + 1), get_16bit_from_mram(program_counter + 3));
            program_counter += 5;
            return;

        case 0xFF:
            hlt();
            return;
        
        default:
            printf("Unknown instruction 0x%02X, halting.\n", instruction);
            cpu_active = false;
            return;
    }
}
