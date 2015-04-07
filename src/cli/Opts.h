#pragma once
#include "../global.h"

class Opts {
public:
  enum Targets {X86, JS};
  enum Stages {PARSE, SEMANTIC, IR, CODEGEN};
  int target = X86;
  string ofile;
  int nonopts;
  int stage = CODEGEN;
  Opts(int argc, char *argv[]);
};
