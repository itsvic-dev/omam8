#include <core.h>
#include <rom.h>

int main() {
    omam8::Core::init();
    omam8::Core::load_rom(omam8::ROM::get_rom("test.rom"));
    omam8::Core::start_loop();
    return 0;
}
