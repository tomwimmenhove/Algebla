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
  #include "../defaulthelper.h"
  #include "../mathop/algeblah.h"
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
                   //END        0  "end of file"
                   END        0  "end"
                   MINUS         "-"
                   PLUS          "+"
                   CARET         "^"
                   STAR          "*"
                   SLASH         "/"
                   LPAREN        "("
                   RPAREN        ")"
                   SEMICOLON     ";"
                   COLON         ":"
                   EQUALS        "="
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
                   SHOW          "show"
                   CLEAR         "clear"
                   HELP          "help"
                   QUESTION      "?"
                   WARRANTY      "warranty"
                   PRECISION     "precision"
                   DIGITS        "digits"
    <number>       NUMBER        "number"
    <std::string>  IDENTIFIER    "identifier"
;

%type  <std::shared_ptr<MathOps::MathOp<number>>> expression
%type  <std::shared_ptr<MathOps::MathOp<number>>> statement
%type  <std::shared_ptr<MathOps::MathOp<number>>> assignment

%right "=";
%right "==";
%left "-" "+"
%left "*" "/"
%precedence POSNEG
%right "^"

%%
%start entries;

entries     : %empty
            | statement
            | entries ";" statement
            | "show"                          { drv.show_variables(); }
            | "clear"                         { drv.clear_variables(); }
            | "help"                          { drv.help(); }
            | "?"                             { drv.help(); }
            | "warranty"                      { drv.warranty(); }
            ;

statement   : expression                      { drv.print_result($1); }
            | assignment                      { drv.print_result($1); }
            ;

assignment  : "identifier" "=" expression     { $$ = drv.assign($1, $3); }
            ;

expression  : "number"                        { $$ = MathOps::MathFactory::ConstantValue<number>($1); }
            | "%pi"                           { $$ = MathOps::MathFactory::SymbolPi<number>(); }
            | "%e"                            { $$ = MathOps::MathFactory::SymbolE<number>(); }
            | "identifier"                    { $$ = drv.find_var($1); }
            | "solve" "identifier"            { drv.make_var($2); } 
              ":" expression "=" expression   { $$ = drv.solve($5, $7, $2); }
            | expression "+" expression       { $$ = $1 + $3; }
            | expression "-" expression       { $$ = $1 - $3; }
            | expression "*" expression       { $$ = $1 * $3; }
            | expression "/" expression       { $$ = $1 / $3; }
            | "-" expression %prec POSNEG     { $$ = -$2; }
            | "+" expression %prec POSNEG     { $$ = $2; }
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
  if (drv.input_is_file())
  {
    std::cerr << l << ": " << m << "\n";
  }
  else
  {
    std::cerr << "Column " << l.begin.column << '-' << l.end.column << ": " << m << "\n";
  }
}

