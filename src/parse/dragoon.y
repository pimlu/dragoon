%code requires {
#include "../global.h"
#include "ast.h"
using namespace ast;

extern bool errs_;
extern string path_;
extern Module *program_;
}

%{
#include <iostream>
#include <cstdio>
using namespace std;

//stuff from flex
extern "C" int yylex();
extern int yylineno;

void yyerror(const char *s);


#include "ast.h"
using namespace ast;

bool errs_ = false;
string path_;
Module *program_;

#define tpos(token) (*reinterpret_cast<TokenPos*>(&token))

%}

%initial-action { errs_ = false; yylineno = 1; }

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
  SimpleControl *control;
  int token;
}

%token <ival> VINT
%token <fval> VFLOAT
%token <sval> VSTRING

%token <sval> BADTOK
%token <token> MODULE IF WHILE ELSE DO
%token <token> EQUAL PLUS MINUS MUL DIV

%type <module> module
%type <block> block
%type <stmts> stmts
%type <stmt> stmt else
%type <expr> expr
%type <control> control
%type <token> binop controltok

%destructor { delete $$; } <sval> <module> <block> <stmt> <expr>
%destructor {
  //just use block's destructor
  delete (new Block(tpos(@$), $$));
  } <stmts>

//operator precedence
%right EQUAL
%left PLUS MINUS
%left MUL DIV
//letting bison know how many s/r conflicts it should be seeing- which is
//exactly the number of operators atm
//%expect 5

%%

dragoon : module { program_ = $1; }
        ;

module : MODULE VSTRING ';' block { $$ = new Module(tpos(@$), $2, $4); delete $2; }
       ;

//TODO more accurate block range
block : stmts { $$ = new Block(tpos(@$), $1); }
      | { $$ = new Block(tpos(@$), new std::vector<Statement*>); }

stmts : stmt { $$ = new std::vector<Statement*>; $$->push_back($1); }
      | stmts stmt { $$ = $1; $$->push_back($2); }
      ;

stmt : expr ';' { $$ = $1; }
     | '{' block '}' { $$ = $2; }
     | error ';' { yyerrok; $$ = nullptr; }
     | control { $$ = $1; }
     | control else { $$ = $1; $1->elsebody = $2; }
     ;

control : controltok '(' expr ')' stmt { $$ = new SimpleControl(tpos(@$), $1, $3, $5); }

else : ELSE stmt { $$ = $2; }

controltok : IF | WHILE
        ;

expr : VINT { $$ = new Int32Expr(tpos(@$), $1); }
     | VSTRING { $$ = new IdExpr(tpos(@$), $1); delete $1; }
     | expr binop expr { $$ = new BinOp(tpos(@$), $2, $1, $3); }
     ;
binop : PLUS | MINUS | MUL | DIV | EQUAL
      ;
%%

void yyerror(const char *s) {
  errs_ = true;
  cout << path_ << ":" << yylloc.first_line << ":" << yylloc.first_column
    << ": error:" << endl<< s << endl;
}
