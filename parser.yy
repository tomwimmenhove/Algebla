%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.0"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <string>
  #include <iostream>
  #include <vector>
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
                   END        0  "end"
                   MINUS         "-"
                   PLUS          "+"
                   CARET         "^"
                   STAR          "*"
                   STAR_ALT      "×"
                   SLASH         "/"
                   LPAREN        "("
                   RPAREN        ")"
                   SEMICOLON     ";"
                   COLON         ":"
                   COMMA         ","
                   EQUALS        "="
                   LAMBDA        "=>"
                   PERCENT       "%"
                   SOLVE         "solve"
                   QUESTION      "?"
                   PRECISION     "precision"
                   DIGITS        "digits"
    <number>       NUMBER        "number"
    <std::string>  IDENTIFIER    "identifier"
;

%type  <std::shared_ptr<MathOps::MathOp<number>>> expression
%type  <std::vector<std::shared_ptr<MathOps::MathOp<number>>>> expressions
%type  <std::shared_ptr<MathOps::MathOp<number>>> assignment
%type  <std::shared_ptr<MathOps::MathOp<number>>> lambda

%right "=";
%right "==";
%left "-" "+"
%left "*" "×" "/"
%precedence POSNEG
%right "^"

%%
%start entries;

entries     : %empty
            | statement
            | delete
            | entries ";" statement
            | ":""identifier"                 { drv.command($2); }
            ;

statement   : expression                      { drv.result($1); }
            | assignment                      { drv.result($1); }
            | lambda                          { drv.result($1); }
            ;

assignment  : "identifier" "=" expression     { $$ = drv.assign($1, $3); }
            ;

lambda      : "identifier" "=>" expression    { $$ = drv.assign_lambda($1, $3); }
            ;

delete      : "identifier" "="                { drv.remove($1); }
            ;
expressions : %empty                          { }
            | expression                      { $$.push_back($1); }
            | expressions "," expression      { $1.push_back($3); $$ = $1; }
            ;

expression  : "number"                        { $$ = MathOps::ConstantValue<number>::create($1); }
            | "%""identifier"                 { $$ = drv.get_constant($2); }
            | "identifier"                    { $$ = drv.find_identifier($1); }
            | "solve" "identifier"            { drv.make_var($2); } 
              ":" expression "=" expression   { $$ = drv.solve($5, $7, $2); }
            | expression "+" expression       { $$ = $1 + $3; }
            | expression "-" expression       { $$ = $1 - $3; }
            | expression "*" expression       { $$ = $1 * $3; }
            | expression "×" expression       { $$ = $1 * $3; }
            | expression "/" expression       { $$ = $1 / $3; }
            | "-" expression %prec POSNEG     { $$ = -$2; }
            | "+" expression %prec POSNEG     { $$ = $2; }
            | expression "^" expression       { $$ = MathOps::pow<number>($1, $3); }
            | "(" expression ")"              { $$ = $2; }
            | "identifier"                    { drv.check_function($1); }
              "(" expressions ")"             { $$ = drv.function($1, $4); }
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

