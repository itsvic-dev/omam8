#ifndef ROM_H
#define ROM_H

#include <string>

namespace omam8 {
    namespace ROM {
        typedef struct __attribute__((packed)) {
            char banks; // 1-64 (inclusive)
            uint8_t *data;
        } ROMData;

        ROMData get_rom(std::string file);
    }
}

#endif // ROM_H
