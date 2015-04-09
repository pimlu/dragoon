#pragma once
#include "../global.h"
#include "Symbol.h"
#include <map>

class Scope {
public:
  Scope *parent = nullptr;
  std::map<string, Symbol*> table;
  Scope();
  Scope(Scope *parent);
  ~Scope();

  Symbol* lookup(string id);
};
