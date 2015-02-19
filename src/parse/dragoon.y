%{
#include <iostream>
#include <cstdio>
using namespace std;

//stuff from flex
extern "C" int yylex();

void yyerror(const char *s);
%}

//C union for the different token types
%union {
  int ival;
  float fval;
  char *sval;
}

%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING

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
