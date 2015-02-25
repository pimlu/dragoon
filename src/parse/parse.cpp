#include "../global.h"
#include "dragoon.tab.h"

extern "C" FILE *yyin;

void parse(const char *path) {
  // open a file handle to a particular file:
  FILE *file = fopen(path, "r");
  // make sure it is valid:
  if (!file) {
    cout << "Unable to open " << path << endl;
    exit(1);
  }
  // set flex to read from it instead of defaulting to STDIN:
  yyin = file;

  // parse through the input until there is no more:
  do {
    yyparse();
  } while (!feof(yyin));

}
