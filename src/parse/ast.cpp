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
template<typename T, typename U>
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
//creates the reverse of our 1:1 map
StrTok reverse(TokStr tokstr) {
  StrTok strtok;
  for(auto& x : tokstr) {
    strtok[x.second] = x.first;
  }
  return strtok;
}

//deletes everything in a vector, and then the vector.
template<typename T>
void deleteVec(std::vector<T*> *vec) {
  for(T *v : *vec) delete v;
  delete vec;
}

TokStr tokstr = create_map<int, string>
  (IF,"if")
  (WHILE,"while")
  (DO,"do")
  (INT,"int")
  (EQUAL,"=")
  (PLUS,"+")
  (MINUS,"-")
  (MUL,"*")
  (DIV,"/");
StrTok strtok = reverse(tokstr);

std::ostream& operator<<(std::ostream& str, Node const& data) {
  data.print(str);
  return str;
}

Node::Node(TokenPos pos) : Node(pos.fline, pos.fcol, pos.lline, pos.lcol) {}

Node::Node(int fline, int fcol, int lline, int lcol) :
  fline(fline), fcol(fcol), lline(lline), lcol(lcol) {}

INITLIST_CPP(Stmt,Node);
INITLIST_CPP(Expr,Stmt);


EmptyStmt::EmptyStmt(TokenPos pos) : Stmt(pos) {}
void EmptyStmt::print(std::ostream& strm) const {
  strm << "<emptystmt>";
}

Block::Block(TokenPos pos, std::vector<Stmt*> *stmts)
  : Stmt(pos), stmts(stmts) {}
Block::~Block() {
  deleteVec<Stmt>(stmts);
}
void Block::print(std::ostream& strm) const {
  strm << "<block";
  tabl++;
  for(Stmt* s : *stmts) {
    strm << tabl << *s;
  }
  strm << --tabl << ">";
}


INITLIST_CPP(Type,Node);

TInt::TInt(TokenPos pos, bool text, bool sign, int size)
  : Type(pos), text(text), sign(sign), size(size) {}
void TInt::print(std::ostream& strm) const {
  strm << "<tint " << (text?"char ":"") << (sign?"signed ":"unsigned ") << size << ">";
}

VarDecl::VarDecl(TokenPos pos, Type *type, std::vector<Expr*> *exprs)
: Stmt(pos), type(type), exprs(exprs) {}
VarDecl::~VarDecl() {
  delete type;
  deleteVec<Expr>(exprs);
}
void VarDecl::print(std::ostream& strm) const {
  strm << "<var " << *type << " ";
  for(Expr* e : *exprs) {
    strm << *e << ",";
  }
  strm << ">";
}

SimpleControl::SimpleControl(TokenPos pos, int type, Expr *cond, Stmt *body)
: Stmt(pos), type(type), cond(cond), body(body) {}
SimpleControl::~SimpleControl() {
  delete cond;
  delete body;
  delete elsebody;
}
void SimpleControl::print(std::ostream& strm) const {
  strm << "<" << tokstr[type] << " (" << *cond << ") " <<
    (type==IF?"then":"do")
    <<" " << *body;
  if(elsebody) {
    strm << tabl << "else " << *elsebody;
  }
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
  strm << "<binop " << tokstr[op] << " " << *lhs << ", " << *rhs << ">";
}

void FuncCall::print(std::ostream& strm) const {
  strm << "<call " << *func << " (";
  bool first = true;
  for(Expr *e : *args) {
    if(first) first = false;
    else strm << ", ";
    strm << *e;
  }
  strm << ") >";
}
FuncCall::FuncCall(TokenPos pos, Expr *func, std::vector<Expr*> *args)
  : Expr(pos), func(func), args(args) {}

FuncCall::~FuncCall() {
  delete func;
  deleteVec<Expr>(args);
}

Param::Param(Type *type, IdExpr *id) : type(type), id(id) {}
Param::~Param() {
  delete type;
  delete id;
}

Func::Func(TokenPos pos, Type *type, IdExpr *id,
  std::vector<Param*> *params, Block *body)
  : Stmt(pos), type(type), id(id), params(params), body(body) {}
Func::~Func() {
  delete type;
  delete id;
  deleteVec<Param>(params);
  delete body;
}
void Func::print(std::ostream& strm) const {
  strm << "<func " << *type << " " << *id << " (";
  bool first = true;
  for(Param *p : *params) {
    if(first) first = false;
    else strm << ", ";
    strm << *p->type << " " << *(p->id);
  }
  strm << ") " << *body;
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
