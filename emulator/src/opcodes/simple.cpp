#include <opcodes/simple.h>
#include <core.h>

void omam8::Opcodes::nop(uint8_t *args) {
    (void *) args;
}

void omam8::Opcodes::hlt(uint8_t *args) {
    (void *) args;
    omam8::Core::halt_cpu();
}
