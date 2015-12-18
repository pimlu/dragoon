#include "ScopeStack.h"

ScopeStack::ScopeStack() {}
shared_ptr<Symbol> ScopeStack::lookup(string id) {
  size_t l = stack.size()-1;
  //repeatedly lookup, starting at the top level
  for(int i = l; i>=0; i--) {
    try {
      return stack[i]->at(id);
    } catch(const std::out_of_range& oor) {}
  }
  return shared_ptr<Symbol>(); //return null otherwise
}
bool ScopeStack::assign(string id, shared_ptr<Symbol> symbol) {
  Scope table = *stack.back();
  
  //try to look up in ours
  try {
    table.at(id);
    return false;
  } catch(const std::out_of_range& oor) {}
  
  table[id] = symbol;
  return true;
}
bool ScopeStack::reassign(string id, shared_ptr<Symbol> symbol) {
  size_t l = stack.size()-1;
  for(int i = l; i>=0; i--) {
    try { //if stack[i] doesn't have id, it throws an error instead of being written
      stack[i]->at(id) = symbol;
      return true;
    } catch(const std::out_of_range& oor) {}
  }
  return false;
}
void ScopeStack::push() {
  stack.push_back(new Scope());
}
void ScopeStack::pop() {
  delete stack.back();
  stack.pop_back();
}
