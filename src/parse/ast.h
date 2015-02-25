#pragma once
#include "../global.h"
#include <vector>

#define INITLIST_H(name)  name();\
  name(TokenPos pos);\
  name(int fline, int fcol, int lline, int lcol);

namespace ast {

//this is layout compatible with YYLTYPE according to C++11, thus
//reinterpret_cast is guaranteed to work correctly
struct TokenPos {
  int fline, fcol, lline, lcol;
};

class Node {
protected:
  virtual void print(std::ostream& strm) const = 0;
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
  std::vector<Statement*> stmts;
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
  unsigned char op;
  bool left = false;
  Expr *lhs, *rhs;
};

class Module : Node {
protected:
  virtual void print(std::ostream& strm) const;
public:
  string name;
  Block *globals;
  Module(TokenPos pos, string name, Block *globals);
};


}