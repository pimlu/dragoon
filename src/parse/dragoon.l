%{
#include <cstdio>
#include <iostream>
using namespace std;
#define YY_DECL extern "C" int yylex()

#include "dragoon.tab.h"  //for tokens

%}
/* noyywrap so that it doesn't call yywrap upon EOF- we do that for it. noinput
   and nounput because we aren't using those functions, causing warnings */
%option noyywrap noinput nounput
%%
[ \t\n]           ;
[0-9]+\.[0-9]+    { yylval.fval = atof(yytext); return FLOAT; }
[0-9]+            { yylval.ival = atoi(yytext); return INT; }
[a-zA-Z0-9]+      {
  yylval.sval = strdup(yytext);
  return STRING;
}
.                ;
%%
