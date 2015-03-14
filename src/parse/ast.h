#pragma once
#include "../global.h"
#include <vector>
#include <map>

#define INITLIST_H(name)  name();\
  name(TokenPos pos);\
  name(int fline, int fcol, int lline, int lcol);

namespace ast {

//converting between token IDs and strings
using TokStr = std::map<int, string>;
using StrTok = std::map<string, int>;
extern TokStr tokstr;
extern StrTok strtok;

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

class Stmt : public Node {
public:
  INITLIST_H(Stmt)
};

class Expr : public Stmt {
public:
  /// Whether it can be used as a left hand for assignment
  bool left;
  INITLIST_H(Expr)
};

class Block : public Stmt {
protected:
  virtual void print(std::ostream& strm) const;
public:
  std::vector<Stmt*> *stmts;
  Block(TokenPos pos, std::vector<Stmt*> *stmts);
  ~Block();
};

//TODO more complex types
using Type = int;


class VarDecl : public Stmt {
protected:
  virtual void print(std::ostream& strm) const;
public:
  Type type;
  std::vector<Expr*> *exprs;
  VarDecl(TokenPos pos, Type type, std::vector<Expr*> *exprs);
  ~VarDecl();
};

class SimpleControl : public Stmt {
protected:
  virtual void print(std::ostream& strm) const;
public:
  int type;
  Expr *cond;
  Stmt *body, *elsebody = nullptr;
  SimpleControl(TokenPos pos, int type, Expr *cond, Stmt *body);
  ~SimpleControl();
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

class Param {
public:
  Type type;
  IdExpr *id;
  Param(Type type, IdExpr *id);
  ~Param();
};


class Func : public Stmt {
protected:
  virtual void print(std::ostream& strm) const;
public:
  Type type;
  IdExpr *id;
  std::vector<Param*> *params;
  Block *body;
  Func(TokenPos pos, Type type, IdExpr *id, std::vector<Param*> *params, Block *body);
  ~Func();
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
