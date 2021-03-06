%code requires {
#include "../global.h"
#include "ast.h"
#include "../cli/error.h"
using namespace ast;

extern bool errs_;
extern string path_;
extern Module *program_;
}

%{
#include <sstream>
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
  std::vector<Stmt*> *stmts;
  std::vector<Expr*> *exprs;
  Stmt *stmt;
  Expr *expr;
  IdExpr *id;
  std::vector<Param*> *params;
  SimpleControl *control;
  int token;
  Type *type;
  std::vector<Type*> *types;
}

%token <ival> VINT "integer"
%token <fval> VFLOAT "float"
%token <sval> VSTRING "identifier"

%token <sval> BADTOK "bad token"
%token <token> MODULE IF WHILE ELSE DO FUNC
%token <token> CHAR INT SHORT LONG UNSIGNED
%token <token> EQUAL PLUS MINUS MUL DIV

%type <module> module
%type <block> block
%type <stmts> stmts
%type <exprs> exprs
%type <stmt> stmt function else
%type <params> params paramlist
%type <expr> expr
%type <id> id
%type <control> control
%type <token> binop controltok
%type <type> type primitive integer
%type <types> types typelist

%destructor { free($$); } <sval>
%destructor { delete $$; } <module> <block> <stmt> <expr> <id> <control> <type>
%destructor {
  for(auto i : *$$) {
    delete i;
  }
  delete $$;
  } <stmts> <exprs> <params> <types>
//operator precedence
%left '(' ')'
%right EQUAL
%left PLUS MINUS
%left MUL DIV
/*letting bison know how many s/r conflicts it should be seeing- which is
5 for operators, 1 for conditionals, 1 for (x+y)() vs x+y(), 1 for
function exprs vs defs */
%expect 8
%%

dragoon : module { program_ = $1; }
        ;

module : MODULE VSTRING ';' stmts {
          $$ = new Module(tpos(@$), $2, errs_, new Block(tpos(@$), $4));
          free($2);
          }
       ;

function : type id '(' params ')' block {
            $$ = new Func(tpos(@$), $1, $2, $4, $6);
            }

params : { $$ = new std::vector<Param*>; }
       | paramlist { $$ = $1; }
       ;

paramlist : type id { $$ = new std::vector<Param*>; $$->push_back(new Param($1, $2)); }
          | paramlist ',' type id { $$ = $1; $$->push_back(new Param($3, $4)); }
          ;

block : '{' stmts '}' { $$ = new Block(tpos(@$), $2); }

stmts : { $$ = new std::vector<Stmt*>; }
      | stmts stmt { $$ = $1; $$->push_back($2); }
      ;

stmt : error ';' { yyerrok; $$ = new BadStmt(tpos(@$)); }
     | function { $$ = $1; }
     | type exprs ';' { $$ = new VarDecl(tpos(@$), $1, $2); }
     | ';' { $$ = new EmptyStmt(tpos(@1)); }
     | expr ';' { $$ = $1; }
     | block { $$ = $1; }
     | control { $$ = $1; }
     | control else { $$ = $1; $1->elsebody = $2; }
     ;

type : '(' type ')' { $$ = $2; }
     | primitive { $$ = $1; }
     | type FUNC '(' types ')' { $$ = new TFunc(tpos(@$), $1, $4); }
     ;

types : { $$ = new std::vector<Type*>; }
      | typelist { $$ = $1; }
      ;

typelist : type { $$ = new std::vector<Type*>; $$->push_back($1); }
         | typelist ',' type { $$ = $1; $$->push_back($3); }
         ;

primitive : integer { $$ = $1; }
          | CHAR { $$ = new TInt(tpos(@$), true, true, 1); }
          | UNSIGNED CHAR { $$ = new TInt(tpos(@$), true, false, 1); }
          ;
integer   : INT { $$ = new TInt(tpos(@$), false, true, 4); }
          | SHORT optint { $$ = new TInt(tpos(@$), false, true, 2); }
          | UNSIGNED SHORT optint { $$ = new TInt(tpos(@$), false, false, 2); }
          | LONG optint { $$ = new TInt(tpos(@$), false, true, 8); }
          | UNSIGNED LONG optint { $$ = new TInt(tpos(@$), false, false, 8); }
          ;
optint : INT | ;

exprs : expr { $$ = new std::vector<Expr*>; $$->push_back($1); }
      | exprs ',' expr { $$ = $1; $$->push_back($3); }

control : controltok '(' expr ')' stmt { $$ = new SimpleControl(tpos(@$), $1, $3, $5); }

else : ELSE stmt { $$ = $2; }

controltok : IF | WHILE
           ;

expr : '(' expr ')' { $$ = $2; }
     | VINT { $$ = new Int32Expr(tpos(@$), $1); }
     | id { $$ = $1; }
     | expr binop expr { $$ = new BinOp(tpos(@$), $2, $1, $3); }
     | expr '(' ')' { $$ = new FuncCall(tpos(@$), $1, new std::vector<Expr*>); }
     | expr '(' exprs ')' { $$ = new FuncCall(tpos(@$), $1, $3); }
     ;

id : VSTRING { $$ = new IdExpr(tpos(@$), $1); free($1); }
   ;

binop : PLUS | MINUS | MUL | DIV | EQUAL
      ;
%%

void yyerror(const char *s) {
  errs_ = true;
  ostringstream msg;
  msg << s;
  if(yychar == BADTOK) {
    msg << " '" << yylval.sval << "'";
  }

  errormsg(path_, tpos(yylloc), msg.str());
}
