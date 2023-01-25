#include "preprocessor.h"
// preprocessor MUST BE before parser.c, as it depends on some preprocessor
// functions
#include "parser.c"
#include <cstdio>

int main() {
  while (yyparse())
    ;
  Preprocessor &preprocessor = get_preprocessor();
  preprocessor.build_intermediate_rom();
  preprocessor.save_rom("test.rom");
  return 0;
}
