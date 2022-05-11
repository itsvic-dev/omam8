#include <cstdio>
#include "preprocessor.h"
#include "parser.c"

int main() {
    while (yyparse());
    return 0;
}
