#pragma once
#include "../global.h"
#include <vector>
#include <map>

#define INITLIST_H(name)  name();\
  name(TokenPos pos);\
  name(int fline, int fcol, int lline, int lcol);

namespace ast {

//converting between token IDs and strings
using TokOp = std::map<int, string>;
using OpTok = std::map<string, int>;
extern TokOp tokop;
extern OpTok optok;

//this is layout compatible with YYLTYPE, thus reinterpret_cast is guaranteed
//to work correctly
struct TokenPos {
  int fline, fcol, lline, lcol;
};

class Node {
protected:
  virtual void print(std::ostream& strm) const = 0;
  virtual ~Node() {};
public:
  int fline, fcol, lline, lcol;

  friend std::ostream& operator<<(std::ostream& str, Node const& data);
  INITLIST_H(Node)
};

class Statement : public Node {
public:
  INITLIST_H(Statement)
};

class Block : public Statement {
protected:
  virtual void print(std::ostream& strm) const;
public:
  std::vector<Statement*> *stmts;
  Block(TokenPos pos, std::vector<Statement*> *stmts);
  ~Block();
};

class Expr : public Statement {
public:
  /// Whether it can be used as a left hand for assignment
  bool left;
  INITLIST_H(Expr)
};

class IdExpr : public Expr {
protected:
  virtual void print(std::ostream& strm) const;
public:
  bool left = true;
  string id;
  IdExpr(TokenPos pos, string id);
};

class Int32Expr : public Expr {
protected:
  virtual void print(std::ostream& strm) const;
public:
  bool left = false;
  int32 val;
  Int32Expr(TokenPos pos, int32 val);
};

class BinOp : public Expr {
protected:
  virtual void print(std::ostream& strm) const;
public:
  int op;
  bool left = false;
  Expr *lhs, *rhs;
  BinOp(TokenPos pos, int op, Expr *lhs, Expr *rhs);
  ~BinOp();
};

class Module : public Node {
protected:
  virtual void print(std::ostream& strm) const;
public:
  string name;
  Block *globals;
  Module(TokenPos pos, string name, Block *globals);
  ~Module();
};


}
