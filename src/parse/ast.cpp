#include "ast.h"
#include "dragoon.tab.h"

#define INITLIST_CPP(name,base) name::name(TokenPos pos) :\
    name(pos.fline, pos.fcol, pos.lline, pos.lcol) {}\
  name::name(int fline, int fcol, int lline, int lcol) :\
    base(fline, fcol, lline, lcol) {}

namespace ast {

class Indent {
public:
  int depth = 0;
  Indent(int depth) : depth(depth) {}
  friend std::ostream& operator<<(std::ostream& str, Indent const& indent) {
    str << endl;
    for(int i = 0; i < indent.depth; i++) {
      str << "  ";
    }
    return str;
  }
  Indent& operator++() {
    depth++;
    return *this;
  }
  Indent& operator--() {
    depth--;
    return *this;
  }
  Indent operator++(int) {
    Indent tmp(*this);
    operator++();
    return tmp;
  }
  Indent operator--(int) {
    Indent tmp(*this);
    operator--();
    return tmp;
  }
};

Indent tabl(0);

//provides boost like functionality by using clever operator overloading
//and templates
template <typename T, typename U>
class create_map {
private:
  std::map<T, U> m_map;
public:
  create_map(const T& key, const U& val) {
    m_map[key] = val;
  }
  create_map<T, U>& operator()(const T& key, const U& val) {
    m_map[key] = val;
    return *this;
  }
  operator std::map<T, U>() {
    return m_map;
  }
};

OpTok reverse(TokOp tokop) {
  OpTok optok;
  for(auto& x : tokop) {
    optok[x.second] = x.first;
  }
  return optok;
}

TokOp tokop = create_map<int, string>
  (EQUAL,"=")
  (PLUS,"+")
  (MINUS,"-")
  (MUL,"*")
  (DIV,"/");
OpTok optok = reverse(tokop);

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
  strm << "<block";
  tabl++;
  for(Statement* s : *stmts) {
    strm << tabl << *s;
  }
  strm << --tabl << ">";
}
Block::~Block() {
  for(Statement *s : *stmts) {
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
  strm << "<binop " << tokop[op] << " " << *lhs << ", " << *rhs << ">";
}

Module::Module(TokenPos pos, string name, Block *globals) : Node(pos),
  name(name), globals(globals) {
}
Module::~Module() {
  delete globals;
}
void Module::print(std::ostream& strm) const {
  strm << "<module " << name << " " << *globals << ">";
}

}
