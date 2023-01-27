#include <core.h>
#include <exception>
#include <iostream>
#include <rom.h>
#include <stdexcept>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "No input ROM specified.\n";
    return 1;
  }

  omam8::Core::init();

  try {
    omam8::Core::load_rom(omam8::ROM::get_rom(argv[1]));
  } catch (std::logic_error exc) {
    std::cerr << "Failed to load ROM: " << exc.what() << "\n";
    return 1;
  }

  try {
    omam8::Core::start_loop();
  } catch (std::logic_error exc) {
    std::cerr << "Execution failed: " << exc.what() << "\n";
    return 1;
  }
  return 0;
}
