#include "Symbol.h"

Symbol::Symbol(string id, Kinds kind, ast::Type *type) : id(id), kind(kind), type(type) {
}
