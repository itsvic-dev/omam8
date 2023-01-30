#include <opcodes/io.h>
#include <core.h>
#include <helpers.h>

void omam8::Opcodes::sioi(uint8_t *args) {
    // sioi: 0b0XXXXXYY: X - pin, Y - IO mode
    uint8_t arg = args[0];
    uint8_t pin = arg >> 2;
    omam8::Core::IOMode mode = static_cast<omam8::Core::IOMode>(arg & 0b11);
    omam8::Core::set_io_pin_mode(pin, mode);
}

void omam8::Opcodes::sior(uint8_t *args) {
    // sior: 0b0XXXXXYY: X - pin, Y - IO mode
    uint8_t arg = omam8::Core::get_register(static_cast<omam8::Core::Register>(args[0]));
    uint8_t pin = arg >> 2;
    omam8::Core::IOMode mode = static_cast<omam8::Core::IOMode>(arg & 0b11);
    omam8::Core::set_io_pin_mode(pin, mode);
}

void omam8::Opcodes::rioia(uint8_t *args) {
    // rioia: 0 - immediate, 1 - address
    uint8_t pin = args[0];
    uint16_t address = join_uint8_to_16(args[1], args[2]);

    bool value = omam8::Core::read_io_pin(pin);
    omam8::Core::set_mram(address, value);
}

void omam8::Opcodes::rioir(uint8_t *args) {
    // rioir: 0 - immediate, 1 - register
    uint8_t pin = args[0];
    omam8::Core::Register reg = static_cast<omam8::Core::Register>(args[1]);

    bool value = omam8::Core::read_io_pin(pin);
    omam8::Core::set_register(reg, value);
}

void omam8::Opcodes::riora(uint8_t *args) {
    // riora: 0 - register, 1 - address
    omam8::Core::Register reg = static_cast<omam8::Core::Register>(args[0]);
    uint8_t pin = omam8::Core::get_register(reg);
    uint16_t address = join_uint8_to_16(args[1], args[2]);

    bool value = omam8::Core::read_io_pin(pin);
    omam8::Core::set_mram(address, value);
}

void omam8::Opcodes::riorr(uint8_t *args) {
    // riorr: 0 - register, 1 - register
    omam8::Core::Register reg1 = static_cast<omam8::Core::Register>(args[0]);
    uint8_t pin = omam8::Core::get_register(reg1);
    omam8::Core::Register reg2 = static_cast<omam8::Core::Register>(args[1]);

    bool value = omam8::Core::read_io_pin(pin);
    omam8::Core::set_register(reg2, value);
}

void omam8::Opcodes::wioai(uint8_t *args) {
    // wioai: 0 - address, 2 - immediate
    uint16_t address = join_uint8_to_16(args[0], args[1]);
    uint8_t pin = args[2];

    uint8_t value = omam8::Core::get_mram(address);
    omam8::Core::write_io_pin(pin, static_cast<bool>(value));
}

void omam8::Opcodes::wioar(uint8_t *args) {
    // wioar: 0 - address, 2 - register
    uint16_t address = join_uint8_to_16(args[0], args[1]);
    uint8_t pin = omam8::Core::get_register(static_cast<omam8::Core::Register>(args[2]));
    
    uint8_t value = omam8::Core::get_mram(address);
    omam8::Core::write_io_pin(pin, static_cast<bool>(value));
}

void omam8::Opcodes::wiori(uint8_t *args) {
    // wiori: 0 - register, 1 - immediate
    uint8_t pin = args[1];
    uint8_t value = omam8::Core::get_register(static_cast<omam8::Core::Register>(args[0]));
    omam8::Core::write_io_pin(pin, static_cast<bool>(value));
}

void omam8::Opcodes::wiorr(uint8_t *args) {
    // wiorr: 0 - register, 1 - register
    uint8_t pin = omam8::Core::get_register(static_cast<omam8::Core::Register>(args[1]));
    uint8_t value = omam8::Core::get_register(static_cast<omam8::Core::Register>(args[0]));
    omam8::Core::write_io_pin(pin, static_cast<bool>(value));
}
