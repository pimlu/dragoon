%code requires {
#include "../global.h"
#include "ast.h"
using namespace ast;
extern Module *program_;
}

%{
#include <iostream>
#include <cstdio>
using namespace std;

//stuff from flex
extern "C" int yylex();

void yyerror(const char *s);


#include "ast.h"
using namespace ast;

Module *program_;

#define tpos(token) (*reinterpret_cast<TokenPos*>(&token))

%}

%locations
%define parse.error verbose

//C union for the different token types
%union {
  //value types- may change type in the future
  int ival;
  float fval;
  char *sval;
  //parse tree related types
  Module *module;
  Block *block;
  std::vector<Statement*> *stmts;
  Statement *stmt;
  Expr *expr;
  int token;
}

%token <ival> VINT
%token <fval> VFLOAT
%token <sval> VSTRING

%token <token> MODULE
%token <token> EQUAL PLUS MINUS MUL DIV

%type <module> module
%type <block> block
%type <stmts> stmts
%type <stmt> stmt
%type <expr> expr
%type <token> binop

//operator precedence
%left PLUS MINUS
%left MUL DIV

%%

dragoon : module { program_ = $1; }
        ;

module : MODULE VSTRING ';' block { $$ = new Module(tpos(@$), $2, $4); }
       ;

//FIXME empty blocks
block : stmts { $$ = new Block(tpos(@$), $1); }

stmts : stmt { $$ = new std::vector<Statement*>; $$->push_back($1); }
      | stmts stmt { $$ = $1; $$->push_back($2); }
      ;

stmt : expr ';' { $$ = $1; }
     | '{' block '}' { $$ = $2; }
     ;

expr : VINT { $$ = new Int32Expr(tpos(@$), $1); }
     | VSTRING { $$ = new IdExpr(tpos(@$), $1); }
     | expr binop expr { $$ = new BinOp(tpos(@$), $2, $1, $3); }
     ;
binop : PLUS | MINUS | MUL | DIV
      ;
%%

void yyerror(const char *s) {
  cout << "Parse error: " << s << endl;
  exit(1);
}
