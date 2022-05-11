#ifndef CORE_H
#define CORE_H

#include <cstdint>
#include <string>

#include <rom.h>

namespace omam8 {
    namespace Core {
        typedef struct {
            std::string displayName;
            unsigned int argsLength;
            void (*handler)(uint8_t *args);
        } Opcode;

        enum Register : unsigned int {
            PC = (1u << 1),
            SP = (1u << 2),
            A = (1u << 3),
            B = (1u << 4),
            C = (1u << 5),
            D = (1u << 6),
        };

        uint8_t get_register(Register reg);
        uint16_t get_combined_register(unsigned int reg);

        void init();
        void handle_opcode();
        void start_loop();

        void load_rom(omam8::ROM::ROMData rom);

        void halt_cpu();
    };
};

#endif // CORE_H
