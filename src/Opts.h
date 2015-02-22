#pragma once
#include "global.h"

class Opts {
public:
  enum Targets {X86,JS};
  int target = X86;
  string ofile;
  int nonopts;
  Opts(int argc, char *argv[]);
};
