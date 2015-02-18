#pragma once
#include "global.h"

class Opts {
public:
  enum Targets {JS,X86};
  int target = JS;
  string ofile;
  int nonopts;
  Opts(int argc, char *argv[]);
};
