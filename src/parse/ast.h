#pragma once
#include "../global.h"
#include <vector>
#include <map>
#include "TokenPos.h"

#define INITLIST_H(name)  name();\
  name(TokenPos pos);\
  name(int fline, int fcol, int lline, int lcol);

namespace ast {

//converting between token IDs and strings
using TokStr = std::map<int, string>;
using StrTok = std::map<string, int>;
extern TokStr tokstr;
extern StrTok strtok;

class Node {
protected:
  virtual void print(std::ostream& strm) const = 0;
  virtual ~Node() {};
public:
  int fline, fcol, lline, lcol;

  friend std::ostream& operator<<(std::ostream& str, Node const& data);
  INITLIST_H(Node)
};



class Type : public Node {
public:
  INITLIST_H(Type);
};

class TInt : public Type {
protected:
  virtual void print(std::ostream& strm) const;
public:
  bool text;
  bool sign;
  int size;
  TInt(TokenPos pos, bool text, bool sign, int size);
};

class TFunc : public Type {
protected:
  virtual void print(std::ostream& strm) const;
public:
  Type *ret;
  std::vector<Type*> *params;
  TFunc(TokenPos pos, Type *ret, std::vector<Type*> *params);
  ~TFunc();
};


class Stmt : public Node {
public:
  INITLIST_H(Stmt)
};

class Expr : public Stmt {
public:
  INITLIST_H(Expr)
};

class EmptyStmt : public Stmt {
protected:
  virtual void print(std::ostream& strm) const;
public:
  EmptyStmt(TokenPos pos);
};

class Block : public Stmt {
protected:
  virtual void print(std::ostream& strm) const;
public:
  std::vector<Stmt*> *stmts;
  Block(TokenPos pos, std::vector<Stmt*> *stmts);
  ~Block();
};

class VarDecl : public Stmt {
protected:
  virtual void print(std::ostream& strm) const;
public:
  Type *type;
  std::vector<Expr*> *exprs;
  VarDecl(TokenPos pos, Type *type, std::vector<Expr*> *exprs);
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
  string id;
  IdExpr(TokenPos pos, string id);
};

class Int32Expr : public Expr {
protected:
  virtual void print(std::ostream& strm) const;
public:
  int32 val;
  Int32Expr(TokenPos pos, int32 val);
};

class BinOp : public Expr {
protected:
  virtual void print(std::ostream& strm) const;
public:
  int op;
  Expr *lhs, *rhs;
  BinOp(TokenPos pos, int op, Expr *lhs, Expr *rhs);
  ~BinOp();
};

class FuncCall : public Expr {
protected:
  virtual void print(std::ostream& strm) const;
public:
  Expr *func;
  std::vector<Expr*> *args;
  FuncCall(TokenPos pos, Expr *func, std::vector<Expr*> *args);
  ~FuncCall();
};

class Param {
public:
  Type *type;
  IdExpr *id;
  Param(Type *type, IdExpr *id);
  ~Param();
};


class Func : public Stmt {
protected:
  virtual void print(std::ostream& strm) const;
public:
  Type *ret;
  IdExpr *id;
  std::vector<Param*> *params;
  Block *body;
  Func(TokenPos pos, Type *ret, IdExpr *id, std::vector<Param*> *params, Block *body);
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
