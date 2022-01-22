%{ /* -*- C++ -*- */
#include <cerrno>
#include <climits>
#include <cstdlib>
#include <string>

#include "../driver.h"
#include "../config.h"
#include "parser.h"

// Work around an incompatibility in flex (at least versions
// 2.5.31 through 2.5.33): it generates code that does
// not conform to C89.  See Debian bug 333231
// <http://bugs.debian.org/cgi-bin/bugreport.cgi?bug=333231>.
# undef yywrap
# define yywrap() 1

// Pacify warnings in yy_init_buffer (observed with Flex 2.6.4)
// and GCC 7.3.0.
#if defined __GNUC__ && 7 <= __GNUC__
# pragma GCC diagnostic ignored "-Wnull-dereference"
#endif
%}

%option noyywrap nounput batch debug noinput

id       [a-zA-Z][a-zA-Z_0-9]*
number   ([0-9]+[.]?[0-9]*|\.[0-9]+)([eE][-+]?[0-9]+)?
blank    [ \t]

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}
{blank}+   loc.step ();
[\n]+      loc.lines (yyleng); loc.step ();

"-"         return yy::parser::make_MINUS     (loc);
"+"         return yy::parser::make_PLUS      (loc);
"*"         return yy::parser::make_STAR      (loc);
"×"         return yy::parser::make_STAR_ALT  (loc);
"^"         return yy::parser::make_CARET     (loc);
"/"         return yy::parser::make_SLASH     (loc);
"("         return yy::parser::make_LPAREN    (loc);
")"         return yy::parser::make_RPAREN    (loc);
";"         return yy::parser::make_SEMICOLON (loc);
":"         return yy::parser::make_COLON     (loc);
","         return yy::parser::make_COMMA     (loc);
"="         return yy::parser::make_EQUALS    (loc);
"=>"        return yy::parser::make_LAMBDA    (loc);
"%"         return yy::parser::make_PERCENT   (loc);
"solve"     return yy::parser::make_SOLVE     (loc);
"?"         return yy::parser::make_QUESTION  (loc);

{number}	{
			errno = 0;
#ifdef ARBIT_PREC
      number d(yytext);
#else
      number d = strtold(yytext, NULL);
#endif
			// XXX: TODO: Check input
			return yy::parser::make_NUMBER (d, loc);
		}

{id}       return yy::parser::make_IDENTIFIER (yytext, loc);

.		{
			throw yy::parser::syntax_error
			(loc, "invalid character: " + std::string(yytext));
		}
<<EOF>>    return yy::parser::make_END (loc);
%%

void driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
}

void driver::scan_file_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "Cannot open " << file << ": " << strerror(errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void driver::scan_file_end ()
{
  fclose (yyin);
}
