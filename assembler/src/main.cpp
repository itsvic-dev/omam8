#include "assembler.h"
// parser.c depends on assembler, preprocessor etc etc
#include "parser.c"
#include <cstdio>
#include <exception>
#include <iostream>

int main(int argc, char **argv) {
  std::string rom = "out.rom";

  if (argc >= 2) {
    rom = argv[1];
  }
  Assembler &assembler = get_assembler();

  try {
    // parse stdin
    while (yyparse())
      ;

    assembler.build_intermediate_rom();
  } catch (std::logic_error exc) {
    std::cerr << "Error: " << exc.what() << "\n";
    return 1;
  }

  assembler.save_rom(rom);
  return 0;
}
