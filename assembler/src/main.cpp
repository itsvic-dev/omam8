#include "preprocessor.h"
// preprocessor MUST BE before parser.c, as it depends on some preprocessor
// functions
#include "parser.c"
#include <cstdio>
#include <exception>
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "No output file specified.\n";
    return 1;
  }
  while (yyparse())
    ;
  Preprocessor &preprocessor = get_preprocessor();
  preprocessor.build_intermediate_rom();
  preprocessor.save_rom(argv[1]);
  return 0;
}
