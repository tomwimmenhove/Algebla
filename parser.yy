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
  #include "algeblah.h"
  class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
# include "driver.h"
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
                   SQRT          "sqrt"
                   LOG           "log"
                   SIN           "sin"
                   ASIN          "asin"
                   COS           "cos"
                   ACOS          "acos"
                   TAN           "tan"
                   ATAN          "atan"
    <double>       NUMBER        "number"
;

//    <std::string>  IDENTIFIER    "identifier"
//%type  <double>                              parameter
//%type  <std::vector<int>>                    parameters
%type  <std::shared_ptr<MathOp<double>>>          exp

/*
%type  <declaration>                        declaration
%type  <statement>                            statement
%type  <std::vector<statement>>                statements
%type  <std::shared_ptr<emjit_function>>    function
%type  <std::vector<emjit_function>>        functions
*/

%%
%start expressions;

expressions : exp             { drv.add_exp($1); };

%left "+" "-";
%left "*" "/";
%right "^";
exp         : exp "+" exp             { $$ = $1 + $3; }
            | exp "-" exp             { $$ = $1 - $3; }
            | exp "*" exp             { $$ = $1 * $3; }
            | exp "/" exp             { $$ = $1 / $3; }
            | exp "^" exp             { $$ = $1 ^ $3; }
            | "(" exp ")"             { $$ = $2; }
            | "sqrt" "(" exp ")"      { $$ = sqrt<double>($3); }
            | "log"  "(" exp ")"      { $$ = log<double>($3); }
            | "sin"  "(" exp ")"      { $$ = sin<double>($3); }
            | "asin" "(" exp ")"      { $$ = asin<double>($3); }
            | "cos"  "(" exp ")"      { $$ = cos<double>($3); }
            | "acos" "(" exp ")"      { $$ = acos<double>($3); }
            | "tan"  "(" exp ")"      { $$ = tan<double>($3); }
            | "atan" "(" exp ")"      { $$ = atan<double>($3); }
            | "number"                { $$ = MathFactory::ConstantValue($1); }
//            | "identifier"  { $$ = expression(expr_type::var, drv.get_var_id($1)); }
            ;
%%

void yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}

