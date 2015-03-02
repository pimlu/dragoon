#include "ast.h"

#define INITLIST_CPP(name,base) name::name(TokenPos pos) :\
    name(pos.fline, pos.fcol, pos.lline, pos.lcol) {}\
  name::name(int fline, int fcol, int lline, int lcol) :\
    base(fline, fcol, lline, lcol) {}

namespace ast {

//TODO replace endl with indenting thing

std::ostream& operator<<(std::ostream& str, Node const& data) {
  data.print(str);
  return str;
}

Node::Node(TokenPos pos) : Node(pos.fline, pos.fcol, pos.lline, pos.lcol) {}

Node::Node(int fline, int fcol, int lline, int lcol) :
  fline(fline), fcol(fcol), lline(lline), lcol(lcol) {}

INITLIST_CPP(Statement,Node);
INITLIST_CPP(Expr,Statement);

Block::Block(TokenPos pos, std::vector<Statement*> *stmts)
  : Statement(pos), stmts(stmts) {}
void Block::print(std::ostream& strm) const {
  strm << "<block" << endl;
  for(Statement* s : *stmts) {
    strm << *s << endl;
  }
  strm << ">";
}
Block::~Block() {
  for(Statement* s : *stmts) {
    delete s;
  }
  delete stmts;
}

IdExpr::IdExpr(TokenPos pos, string id) : Expr(pos), id(id) {}
Int32Expr::Int32Expr(TokenPos pos, int32 val) : Expr(pos), val(val) {}

void IdExpr::print(std::ostream& strm) const {
  strm << "<id " << id << ">";
}
void Int32Expr::print(std::ostream& strm) const {
  strm << "<int32 " << val << ">";
}

BinOp::BinOp(TokenPos pos, int op, Expr *lhs, Expr *rhs)
  : Expr(pos), op(op), lhs(lhs), rhs(rhs) {}
BinOp::~BinOp() {
  delete lhs;
  delete rhs;
}
void BinOp::print(std::ostream& strm) const {
  //FIXME op string
  strm << "<" << op << " " << *lhs << ", " << *rhs << ">";
}

Module::Module(TokenPos pos, string name, Block *globals) : Node(pos),
  name(name), globals(globals) {
}
Module::~Module() {
  delete globals;
}
void Module::print(std::ostream& strm) const {
  strm << "<module " << name << " " << *globals << ">" << endl;
}

}
