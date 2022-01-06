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
  class driver;
  extern void yy_read_input(char *buf, int& result, int max_size);
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
# include "../driver.h"
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
    <double>       NUMBER        "number"
    <std::string>  IDENTIFIER    "identifier"
;

%type  <std::shared_ptr<MathOp<double>>>          expression

%%
%start expressions;

expressions :                            { }
            | expressions expression     { drv.add_exp($2); }
            ;

%left "+" "-";
%left "*" "/";
%right "^";
expression  : expression "+" expression  { $$ = $1 + $3; }
            | expression "-" expression  { $$ = $1 - $3; }
            | expression "*" expression  { $$ = $1 * $3; }
            | expression "/" expression  { $$ = $1 / $3; }
            | expression "^" expression  { $$ = $1 ^ $3; }
            | "(" expression ")"         { $$ = $2; }
            | "sqrt" "(" expression ")"  { $$ = sqrt<double>($3); }
            | "log"  "(" expression ")"  { $$ = log<double>($3); }
            | "sin"  "(" expression ")"  { $$ = sin<double>($3); }
            | "asin" "(" expression ")"  { $$ = asin<double>($3); }
            | "cos"  "(" expression ")"  { $$ = cos<double>($3); }
            | "acos" "(" expression ")"  { $$ = acos<double>($3); }
            | "tan"  "(" expression ")"  { $$ = tan<double>($3); }
            | "atan" "(" expression ")"  { $$ = atan<double>($3); }
            | "number"                   { $$ = MathFactory::ConstantValue<double>($1); }
            | "%pi"                      { $$ = MathFactory::SymbolPi<double>(); }
            | "%e"                       { $$ = MathFactory::SymbolE<double>(); }
            | "identifier"               { $$ = MathFactory::Variable<double>($1); }
            ;
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}

