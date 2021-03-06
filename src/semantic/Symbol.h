#pragma once
#include "../global.h"
#include "../parse/ast.h"

class Symbol {
public:
  string id;
  enum Kinds {VAR, PARAM, TDEF};
  Kinds kind;
  ast::Type *type;
  Symbol(string id, Kinds kind, ast::Type *type);
  
};
