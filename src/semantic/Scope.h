#pragma once
#include "../global.h"
#include "Symbol.h"
#include <map>

class Scope {
public:
  //self is used for reference counting children
  Scope *parent = nullptr;
  std::map<string, Symbol*> table;
  Scope();
  Scope(Scope* parent);
  
  void assign(string id, Symbol *symbol);
  Symbol* lookup(string id);
};
