#include "Scope.h"
#include <stdexcept>

Scope::Scope(Scope *parent) : parent(parent) {
}

void Scope::assign(string id, Symbol *symbol) {
  table[id] = symbol;
}

Symbol* Scope::lookup(string id) {
  Symbol *ret = nullptr;
  //try to look up in ours
  try {
    ret = table.at(id);
  } catch(const std::out_of_range& oor) {}
  //if nothing found, check parent
  if(ret == nullptr && parent != nullptr) {
    ret = parent->lookup(id);
  }
  return ret;
}
