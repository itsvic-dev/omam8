#include <iostream>
#include <boost/program_options.hpp>
#include <version.h>
#include <core.h>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iomanip>

namespace po = boost::program_options;

std::vector<uint8_t> readROM() {
    // open the file
    std::streampos fileSize;
    std::ifstream file("rom.bin", std::ios::binary);
    if (file.fail()) {
        throw 1;
    }

    // get its size
    file.seekg(0, std::ios::end);
    fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // read the data
    std::vector<uint8_t> fileData(fileSize);
    file.read((char*) &fileData[0], fileSize);
    file.close();
    return fileData;
}

void dumpRAM(EmulatorCore core) {
    std::ofstream mram_file("mram.bin", std::ios::out | std::ios::binary | std::ios::trunc);
    mram_file.write((char*) core.mram, 0xFFFF);
    mram_file.close();

    std::ofstream vram_file("vram.bin", std::ios::out | std::ios::binary | std::ios::trunc);
    vram_file.write((char*) core.vram, 0x9600);
    vram_file.close();
}

TTF_Font* Font = nullptr;

void displayVRAM(SDL_Renderer* renderer, EmulatorCore core) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    if (Font == nullptr) {
        Font = TTF_OpenFont("/usr/share/fonts/TTF/JetBrainsMono-Regular.ttf", 14);
    }

    for (int y = 0; y < 160; y++) {
        for (int x = 0; x < 240; x++) {
            int i = y * 240 + x;
            int color = (int) core.vram[i];
            if (color == 0) continue;
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect rect;
    rect.x = 0;
    rect.y = 161;
    rect.w = 240;
    rect.h = 1;
    SDL_RenderFillRect(renderer, &rect);

    SDL_Color White = {255, 255, 255};
    SDL_Color Black = {0, 0, 0};

    std::ostringstream ab;
    ab << "A: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (int) core.reg_a;
    ab << " | B: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(2) << (int) core.reg_b;
    std::ostringstream pcsp;
    pcsp << "PC: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << (int) core.reg_pc;
    pcsp << " | SP: 0x" << std::uppercase << std::hex << std::setfill('0') << std::setw(4) << (int) core.reg_sp;

    SDL_Surface* AB_Surface = TTF_RenderText_Shaded(
        Font, ab.str().c_str(), White, Black
    );

    SDL_Texture* AB_Texture = SDL_CreateTextureFromSurface(renderer, AB_Surface);

    SDL_Rect AB_Rect;
    AB_Rect.x = 16;
    AB_Rect.y = 160 + 16;
    AB_Rect.w = AB_Surface->w;
    AB_Rect.h = AB_Surface->h;
    SDL_RenderCopy(renderer, AB_Texture, NULL, &AB_Rect);

    SDL_Surface* PCSP_Surface = TTF_RenderText_Shaded(
        Font, pcsp.str().c_str(), White, Black
    );

    SDL_Texture* PCSP_Texture = SDL_CreateTextureFromSurface(renderer, PCSP_Surface);

    SDL_Rect PCSP_Rect;
    PCSP_Rect.x = AB_Rect.x;
    PCSP_Rect.y = AB_Rect.y + AB_Rect.h;
    PCSP_Rect.w = PCSP_Surface->w;
    PCSP_Rect.h = PCSP_Surface->h;
    SDL_RenderCopy(renderer, PCSP_Texture, NULL, &PCSP_Rect);

    SDL_FreeSurface(AB_Surface);
    SDL_DestroyTexture(AB_Texture);

    SDL_FreeSurface(PCSP_Surface);
    SDL_DestroyTexture(PCSP_Texture);

    SDL_RenderPresent(renderer);
}

int main(int argc, char** argv) {
    po::options_description optional_args("Arguments");
    optional_args.add_options()
        ("help", "show this help message and exit")
        ("version", "show version info and exit")
        ("verbose", "outputs the instructions processed in omam8 assembler form and dumps contents of MRAM/VRAM on halt")
    ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, optional_args), vm);
    po::notify(vm);

    if (vm.count("version") || vm.count("help")) {
        std::cout << "omam8 emulator v" << OMAM8EMU_VERSION << ", supporting spec v" << SPEC_VERSION << std::endl;
    }

    if (vm.count("help")) {
        std::cout << std::endl << optional_args;
        return 0;
    }

    if (vm.count("version")) {
        return 0;
    }

    bool verbose = false;
    if (vm.count("verbose")) {
        verbose = true;
    }

    std::cout << "Starting omam8 emulator v" << OMAM8EMU_VERSION << "..." << std::endl;

    EmulatorCore core;

    try {
        std::cout << "Initializing emulator core..." << std::endl;
        core.initialize(readROM(), verbose);
    } catch (int exception) {
        switch(exception) {
            case 1:
                std::cerr << "Failed to open file." << std::endl;
                return 1;
            default:
                std::cerr << "Unknown exception has occured." << std::endl;
                return 2;
        }
    }

    std::cout << "Initializing SDL2..." << std::endl;

    int rendererFlags = SDL_RENDERER_ACCELERATED;
    int windowFlags = 0;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "omam8 emulator",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        240, 160 + 64, windowFlags
    );

    if (!window) {
        std::cerr << "Failed to open window: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, rendererFlags
    );

    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        return 1;
    }

    TTF_Init();

    while (true) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_QUIT:
                    return 0;
                default:
                    break;
            }
        }
        try {
            core.run_clock_cycle();
        } catch(int exception) {
            switch(exception) {
                case 0xFF:
                    std::cout << "CPU halted." << std::endl;
                    break;
                default:
                    std::cerr << "Unknown exception has occured, check the log output for more information." << std::endl;
            }

            if (verbose) {
                printf("\n--- REGISTERS ---\nA: 0x%02X\nB: 0x%02X\nR: 0x%02X\nPC: 0x%04X\nSP: 0x%04X\n",
                    core.reg_a, core.reg_b, core.reg_r, core.reg_pc, core.reg_sp);
                dumpRAM(core);
            }
            return 0;
        }
        displayVRAM(renderer, core);
    }

    return 0;
}
