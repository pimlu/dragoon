#include "ast.h"

#define INITLIST_CPP(name,base) name::name(TokenPos pos) :\
    name(pos.fline, pos.fcol, pos.lline, pos.lcol) {}\
  name::name(int fline, int fcol, int lline, int lcol) :\
    base(fline, fcol, lline, lcol) {}

namespace ast {

std::ostream& operator<<(std::ostream& str, Node const& data) {
  data.print(str);
  return str;
}

Node::Node(TokenPos pos) : Node(pos.fline, pos.fcol, pos.lline, pos.lcol) {}

Node::Node(int fline, int fcol, int lline, int lcol) :
  fline(fline), fcol(fcol), lline(lline), lcol(lcol) {}

INITLIST_CPP(Statement,Node);
INITLIST_CPP(Expr,Statement);

void Block::print(std::ostream& strm) const {
  for(Statement* s : stmts) {
    strm << s << endl;
  }
}

IdExpr::IdExpr(TokenPos pos, string id) : Expr(pos), id(id) {}
Int32Expr::Int32Expr(TokenPos pos, int32 val) : Expr(pos), val(val) {}

void IdExpr::print(std::ostream& strm) const {
  strm << "<id " << id << ">";
}
void Int32Expr::print(std::ostream& strm) const {
  strm << val;
}
void BinOp::print(std::ostream& strm) const {
  strm << "<" << op << " " << lhs << "," << rhs << ">";
}

Module::Module(TokenPos pos, string name, Block *globals) : Node(pos),
  name(name), globals(globals) {
}
void Module::print(std::ostream& strm) const {
  strm << "<module " << name << endl << globals << ">" << endl;
}

}
