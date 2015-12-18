#pragma once
#include "../global.h"
#include "Symbol.h"
#include <vector>
#include <map>


using Scope = std::map<string, shared_ptr<Symbol>>;

class ScopeStack {
public:
  std::vector<Scope*> stack;
  ScopeStack();
  shared_ptr<Symbol> lookup(string id);
  //returns whether there was already this id at the highest scope
  bool assign(string id, shared_ptr<Symbol> symbol);
  //returns whether this id was in any scope
  bool reassign(string id, shared_ptr<Symbol> symbol);
  void push();
  void pop();
};
