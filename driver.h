#ifndef DRIVER_HH
#define DRIVER_HH

#include "parser.h"

#include "config.h"

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <map>

// Tell Flex the lexer's prototype ...
# define YY_DECL \
	yy::parser::symbol_type yylex (driver& drv)
// ... and declare it for the parser's sake.
YY_DECL;

class driver
{
public:
	driver ();

	// Run the parser on file f.  Return 0 on success.
	int parse_file(const std::string& f);
	int parse_string(const std::string& line);

	// The name of the file being parsed.
	std::string file;
	// Whether to generate parser debug traces.
	bool trace_parsing;

	// Handling the scanner.
	void scan_begin();
	void scan_file_begin();
	void scan_file_end();
	// Whether to generate scanner debug traces.
	bool trace_scanning;
	// The token's location used by the scanner.
	yy::location location;

	void add_exp(std::shared_ptr<MathOp<number>> exp);
	std::shared_ptr<MathOp<number>> solve(std::shared_ptr<MathOp<number>> op, std::string variable, number result);
	std::shared_ptr<MathOpVariableBase<number>> find_var(std::string variable);
	std::shared_ptr<MathOp<number>> assign(std::string variable, std::shared_ptr<MathOp<number>> op);
	const std::vector<std::shared_ptr<MathOp<number>>> get_expressions() const { return expressions; }
	void make_var(std::string variable);

private:
	std::vector<std::shared_ptr<MathOp<number>>> expressions;
	std::map<std::string, std::shared_ptr<MathOpVariableBase<number>>> variables;
	int var_id = 0;
};
#endif // ! DRIVER_HH
