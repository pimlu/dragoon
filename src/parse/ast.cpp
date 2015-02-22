#include "ast.h"

namespace ast {

std::ostream& operator<<(std::ostream& ostr, const Module& m) {
  return ostr << m.name;
}

}
