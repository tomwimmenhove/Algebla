%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>
  #include <iostream>
  #include <memory>
  #include "../algeblah.h"
  #include "../config.h"
  class driver;
  extern void yy_read_input(char *buf, int& result, int max_size);
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
#include "../driver.h"
}

%define api.token.prefix {TOK_}
%token
                   END        0  "end of file"
                   MINUS         "-"
                   PLUS          "+"
                   CARET         "^"
                   STAR          "*"
                   SLASH         "/"
                   LPAREN        "("
                   RPAREN        ")"
                   SEMICOLON     ";"
                   COLON         ":"
                   ASSIGN        "="
                   EQUALS        "=="
                   E             "%e"
                   PI            "%pi"
                   SQRT          "sqrt"
                   LOG           "log"
                   SIN           "sin"
                   ASIN          "asin"
                   COS           "cos"
                   ACOS          "acos"
                   TAN           "tan"
                   ATAN          "atan"
                   SOLVE         "solve"
    <number>       NUMBER        "number"
    <std::string>  IDENTIFIER    "identifier"
;

%type  <std::shared_ptr<MathOp<number>>>          expression

%right "=";
%right "==";
%left "-" "+"
%left "*" "/"
%precedence POSNEG
%right "^"

%%
%start expressions;

expressions : %empty
            | expression                      { drv.add_exp($1); }
            | expressions ";" expression      { drv.add_exp($3); }
            ;

expression  : "number"                        { $$ = MathFactory::ConstantValue<number>($1); }
            | "%pi"                           { $$ = MathFactory::SymbolPi<number>(); }
            | "%e"                            { $$ = MathFactory::SymbolE<number>(); }
            | "identifier"                    { $$ = drv.find_var($1); }
            | "solve" "identifier"            { drv.make_var($2); } 
              ":" expression "==" expression  { $$ = drv.solve($5, $7, $2); }
            | "identifier" "=" expression     { $$ = drv.assign($1, $3); }
            | expression "+" expression       { $$ = $1 + $3; }
            | expression "-" expression       { $$ = $1 - $3; }
            | expression "*" expression       { $$ = $1 * $3; }
            | expression "/" expression       { $$ = $1 / $3; }
            | "-" expression  %prec POSNEG    { $$ = -$2; }
            | "+" expression  %prec POSNEG    { $$ = $2; }
            | expression "^" expression       { $$ = $1 ^ $3; }
            | "(" expression ")"              { $$ = $2; }
            | "sqrt" "(" expression ")"       { $$ = sqrt<number>($3); }
            | "log"  "(" expression ")"       { $$ = log<number>($3); }
            | "sin"  "(" expression ")"       { $$ = sin<number>($3); }
            | "asin" "(" expression ")"       { $$ = asin<number>($3); }
            | "cos"  "(" expression ")"       { $$ = cos<number>($3); }
            | "acos" "(" expression ")"       { $$ = acos<number>($3); }
            | "tan"  "(" expression ")"       { $$ = tan<number>($3); }
            | "atan" "(" expression ")"       { $$ = atan<number>($3); }
            ;
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << "\n";
}

