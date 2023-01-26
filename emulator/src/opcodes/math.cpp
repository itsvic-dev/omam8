#include <opcodes/math.h>
#include <core.h>

void omam8::Opcodes::addi(uint8_t *args) {
    // addi: 0 - immediate, 1 - register
    uint8_t a = args[0];
    omam8::Core::Register reg = static_cast<omam8::Core::Register>(args[1]);
    uint8_t b = omam8::Core::get_register(reg);
    omam8::Core::set_register(reg, a + b);
}

void omam8::Opcodes::addr(uint8_t *args) {
    // addr: 0 - register, 1 - register
    omam8::Core::Register reg1 = static_cast<omam8::Core::Register>(args[0]);
    omam8::Core::Register reg2 = static_cast<omam8::Core::Register>(args[1]);

    uint8_t a = omam8::Core::get_register(reg1);
    uint8_t b = omam8::Core::get_register(reg2);
    omam8::Core::set_register(reg2, a + b);
}

void omam8::Opcodes::subi(uint8_t *args) {
    // subi: 0 - immediate, 1 - register
    uint8_t a = args[0];
    omam8::Core::Register reg = static_cast<omam8::Core::Register>(args[1]);
    uint8_t b = omam8::Core::get_register(reg);
    omam8::Core::set_register(reg, b - a);
}

void omam8::Opcodes::subr(uint8_t *args) {
    // subr: 0 - register, 1 - register
    omam8::Core::Register reg1 = static_cast<omam8::Core::Register>(args[0]);
    omam8::Core::Register reg2 = static_cast<omam8::Core::Register>(args[1]);

    uint8_t a = omam8::Core::get_register(reg1);
    uint8_t b = omam8::Core::get_register(reg2);
    omam8::Core::set_register(reg2, b - a);
}
