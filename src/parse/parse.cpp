#include "parse.h"

void yyrestart( FILE *new_file );

Module* parse(const char *path) {
  path_ = string(path);
  // open a file handle to a particular file:
  FILE *file = fopen(path, "r");
  // make sure it is valid:
  if (!file) {
    cout << "Unable to open " << path << endl;
    exit(1);
  }
  // set flex to read from it instead of defaulting to STDIN:
  yyrestart(file);
  yyparse();

  if(errs_) {
    delete program_;
    program_ = nullptr;
  }
  return program_;
}
