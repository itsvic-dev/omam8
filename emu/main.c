#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "core.c"

const int SCREEN_WIDTH = 240;
const int SCREEN_HEIGHT = 160;
const int render_every = 1024;
int render_counter = 0;
bool quit = false;

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

    printf(
        "CPU halted.\n\nRegisters:\nPC: 0x%04X\nR: 0x%02X\nSP: 0x%04X\nA: 0x%02X\n",
        program_counter, register_remainder, stack_pointer, register_a
    );

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
                printf("SDL2 quit detected, leaving.\n");
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
