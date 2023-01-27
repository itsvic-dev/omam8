#include <core.h>
#include <iostream>
#include <rom.h>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "No input ROM specified.\n";
    return 1;
  }

  omam8::Core::init();
  omam8::Core::load_rom(omam8::ROM::get_rom(argv[1]));
  omam8::Core::start_loop();
  return 0;
}
