#ifndef DRIVER_HH
#define DRIVER_HH

#include "parser.h"
#include "options.h"
#ifdef GNUPLOT
#include "gnuplot.h"
#endif
#include "config.h"

#include <string>
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <algorithm>

// Tell Flex the lexer's prototype ...
# define YY_DECL \
	yy::parser::symbol_type yylex (driver& drv)
// ... and declare it for the parser's sake.
YY_DECL;

class driver
{
public:
	driver (options opt);

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

	void result(std::shared_ptr<MathOps::MathOp<number>> op);
	std::shared_ptr<MathOps::MathOp<number>> solve(std::shared_ptr<MathOps::MathOp<number>> lhs,
    	std::shared_ptr<MathOps::MathOp<number>> rhs, const std::string& variable, number index);
	void plot(const std::string& variable,
		std::vector<std::shared_ptr<MathOps::MathOp<number>>> equations,
		std::vector<std::shared_ptr<MathOps::MathOp<number>>> args);
	void replot();
	void unplot();
	std::shared_ptr<MathOps::MathOp<number>> find_identifier(const std::string& variable);
	std::shared_ptr<MathOps::MathOp<number>> assign(const std::string& variable, std::shared_ptr<MathOps::MathOp<number>> op);
	std::shared_ptr<MathOps::MathOp<number>> assign_lambda(const std::string& variable, std::shared_ptr<MathOps::MathOp<number>> op);
	void unassign(const std::string& name);
	void make_var(const std::string& variable);

	void check_function(const std::string& func_name);
	std::shared_ptr<MathOps::MathOp<number>> function(const std::string& func_name,
		std::vector<std::shared_ptr<MathOps::MathOp<number>>> ops);
	std::shared_ptr<MathOps::MathOp<number>> get_constant(const std::string& id);
	void command(const std::string& cmd);

	bool input_is_file() const { return is_file; }

private:
	void show_variables();
	void clear_variables();
	void help();
	void warranty();

	void check_reserved(const std::string& variable);
	std::string format(std::shared_ptr<MathOps::MathOp<number>> op);
	std::string result_string(std::shared_ptr<MathOps::MathOp<number>> op, number result);
	number print_result(std::shared_ptr<MathOps::MathOp<number>> op);

	template <typename U>
	static std::shared_ptr<U> get(std::vector<std::shared_ptr<U>>& from, const std::string& name)
	{
		auto it = std::find_if(from.begin(), from.end(), [&](std::shared_ptr<U> v) { return v->get_name() == name; });
	    return it == from.end() ? nullptr : *it;   
	}

	template <typename U>
	void remove(std::vector<std::shared_ptr<U>>& from, std::shared_ptr<MathOps::MathOp<number>> op)
	{
		auto it = std::find(from.begin(), from.end(), op);
		if (it != from.end())
		{
			from.erase(it);
		}

#ifdef GNUPLOT
		delete_plot_using(op);
#endif
	}

	std::shared_ptr<MathOps::Variable<number>> get_var(const std::string& variable);
	std::shared_ptr<MathOps::Container<number>> get_lambda(const std::string& variable);
	
#ifdef GNUPLOT
	void delete_plot_using(std::shared_ptr<MathOps::MathOp<number>> op);

	GnuPlot<number> gp;
	std::string plot_variable;
	std::vector<std::shared_ptr<MathOps::MathOp<number>>> plot_equations;
	std::vector<std::shared_ptr<MathOps::MathOp<number>>> plot_args;
#endif

	options opt;
	bool is_file;
	int var_id = 0;

	std::shared_ptr<MathOps::Variable<number>> digits;
	std::shared_ptr<MathOps::Variable<number>> ans;

#ifdef ARBIT_PREC
	std::shared_ptr<MathOps::Variable<number>> precision;
#endif
	std::vector<std::shared_ptr<MathOps::Variable<number>>> variables;
	std::vector<std::shared_ptr<MathOps::Container<number>>> lambdas;
};
#endif // ! DRIVER_HH
