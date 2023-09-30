#include "assembler.h"
// parser.c depends on assembler, preprocessor etc etc
#include "parser.c"
#include <cstdio>
#include <exception>
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "No output file specified.\n";
    return 1;
  }
  Assembler &assembler = get_assembler();

  // parse stdin
  while (yyparse())
    ;

  try {
    assembler.build_intermediate_rom();
  } catch (std::logic_error exc) {
    std::cerr << "Error: " << exc.what() << "\n";
    return 1;
  }
  assembler.save_rom(argv[1]);
  return 0;
}
