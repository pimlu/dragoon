%code requires {
#include "../global.h"
#include "ast.h"
using namespace ast;

}

%{
#include <iostream>
#include <cstdio>
using namespace std;

//stuff from flex
extern "C" int yylex();

void yyerror(const char *s);

%}

%locations

//C union for the different token types
%union {
  int ival;
  float fval;
  char *sval;
  Module *module;
  Block *block;
  int token;
}

%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING


%token <token> TEQUAL
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE
%token <token> TPLUS TMINUS TMUL TDIV

%%

dragoon:
  dragoon INT      { cout << "int: " << $2 << endl; }
  | dragoon FLOAT  { cout << "float: " << $2 << endl; }
  | dragoon STRING { cout << "string: " << $2 << endl; }
  | INT            { cout << "int: " << $1 << endl; }
  | FLOAT          { cout << "float: " << $1 << endl; }
  | STRING         { cout << "string: " << $1 << endl; }
  ;

%%

void yyerror(const char *s) {
  cout << "Parse error: " << s << endl;
  exit(1);
}
