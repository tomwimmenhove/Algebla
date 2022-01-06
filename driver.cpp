#include <iostream>

#include "driver.h"
#include "parser.h"

using namespace std;

driver::driver ()
  : trace_parsing (false), trace_scanning (false)
{ }

int driver::parse_file (const std::string &f)
{
  file = f;
  location.initialize(&file);
  scan_file_begin();
  yy::parser parser(*this);
  parser.set_debug_level(trace_parsing);
  int res = parser.parse();
  scan_file_end();
  return res;
}

typedef struct yy_buffer_state * YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char * str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern YY_BUFFER_STATE yy_scan_string ( const char *yy_str  );

int driver::parse_string(const std::string& line)
{
  yy_scan_string(line.c_str());
  scan_begin();
  std::string file("string input");
  location.initialize(&file);
  yy::parser parser(*this);
  parser.set_debug_level(trace_parsing);
  return parser.parse();
}

void driver::add_exp(std::shared_ptr<MathOp<double>> exp)
{
	expressions.push_back(exp);
}
