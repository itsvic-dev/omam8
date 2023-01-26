#ifndef MATH_H
#define SIMPLE_H

#include <cstdint>

namespace omam8 {
    namespace Opcodes {
        void addi(uint8_t *args);
        void addr(uint8_t *args);
        void subi(uint8_t *args);
        void subr(uint8_t *args);
    }
}

#endif // MATH_H
