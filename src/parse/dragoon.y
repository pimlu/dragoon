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

%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING

%token <token> MODULE
%token <token> SEMI
%token <token> TEQUAL
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE
%token <token> TPLUS TMINUS TMUL TDIV

%type <module> module
%type <block> block
%type <stmts> stmts
%type <stmt> stmt
%type <expr> expr

%%

dragoon : module { program_ = $1; }
        ;

module : MODULE STRING SEMI block { $$ = new Module(tpos(@$), $2, $4); }
       ;

block : stmts { $$ = new Block(tpos(@$), $1); }

stmts : stmt { $$ = new std::vector<Statement*>; $$->push_back($1); }
      | stmts stmt { $$ = $1; $$->push_back($2); }
      ;

stmt : expr SEMI { $$ = $1; }
     | TLBRACE block TRBRACE { $$ = $2; }
     ;

expr : INT { $$ = new Int32Expr(tpos(@$), $1); }
     ;

%%

void yyerror(const char *s) {
  cout << "Parse error: " << s << endl;
  exit(1);
}
