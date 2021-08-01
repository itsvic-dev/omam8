#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 160;

uint8_t mram[0xFFFF];
uint8_t vram[0x9600];
uint16_t program_counter = 0x8000;
uint8_t register_remainder;
uint16_t stack_pointer;

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

void clr() {
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

void hlt() {
    cpu_active = false;
}

// ----------------------------------------------------------------
// OTHER LOGIC
// ----------------------------------------------------------------

const int render_every = 1024;
int render_counter = 0;
bool quit = false;

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
        
        case 0xFF:
            hlt();
            return;
        
        default:
            printf("Unknown instruction 0x%02X, halting.\n", instruction);
            cpu_active = false;
            return;
    }
}

SDL_Renderer* renderer = NULL;

void update_sdl2_pixels() {
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
        for (int x = 0; x < SCREEN_WIDTH; x++) {
            int i = y * SCREEN_WIDTH + x;
            Uint8 color = (Uint8) vram[i];
            if (color != 0) {
                SDL_SetRenderDrawColor(renderer, color, color, color, 0xFF);
                SDL_RenderDrawPoint(renderer, x, y);
            }
        }
    }
}

int main(void) {
    printf("omam8 emulator\n");
    printf("Initializing...\n");

    // read ROM
    FILE *fileptr;
    uint8_t* buffer;
    int filelen = 0;

    fileptr = fopen("rom.bin", "rb");
    fseek(fileptr, 0, SEEK_END);
    filelen = (int) ftell(fileptr);
    if (filelen > 0x8000) {
        fclose(fileptr);
        fprintf(stderr, "ROM is too big (>0x8000 bytes).");
        return 1;
    }
    rewind(fileptr);

    buffer = (uint8_t*) malloc(filelen * sizeof(uint8_t));
    fread(buffer, filelen, 1, fileptr);
    fclose(fileptr);

    for (int i = 0; i < filelen; i++) {
        mram[0x8000 + i] = buffer[i];
    }

    free(buffer);

    // init SDL2
    SDL_Window* window = NULL;

    SDL_Surface* screenSurface = NULL;

    printf("Starting SDL2...\n");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL couldn't initialize: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "omam8 emulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (window == NULL) {
        fprintf(stderr, "Window couldn't be created: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_Event event;

    printf("Starting loop...\n");
    while (cpu_active) {
        SDL_PollEvent(&event);
        
        execute();

        switch (event.type) {
            case SDL_QUIT:
            printf("SDL2 quit detected, halting CPU.\n");
                cpu_active = false;
                quit = true;
                break;
        }

        render_counter ++;
        if (render_counter == render_every) {
            SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
            SDL_RenderClear(renderer);
            update_sdl2_pixels();
            SDL_RenderPresent(renderer);
            render_counter = 0;
        }
    }

    printf("CPU halted.\n\nRegisters:\nPC: 0x%04X\nR: 0x%02X\nSP: 0x%04X\n", program_counter, register_remainder, stack_pointer);

    printf("\nDumping RAM... ");

    FILE* mramptr;
    mramptr = fopen("mram.bin", "wb+");
    fwrite(mram, sizeof(mram), 1, mramptr);
    fclose(mramptr);

    FILE* vramptr;
    vramptr = fopen("vram.bin", "wb+");
    fwrite(vram, sizeof(vram), 1, vramptr);
    fclose(vramptr);

    printf("Done.\n");

    if (!quit) {
        printf("Waiting for user to close...\n");
        while (!quit) {
            SDL_WaitEvent(&event);

            switch (event.type) {
                case SDL_QUIT:
                printf("SDL2 quit detected, leaving.");
                    quit = true;
                    break;
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
