#include <iostream>
#include <algorithm>

#include "driver.h"
#include "parser.h"
#include "mathop/findvariabetransformer.h"
#include "mathop/rearrangetransformer.h"
#include "mathop/defaultformatter.h"
#include "usefulfraction.h"

using namespace std;

driver::driver(options opt)
    : opt(opt),
      trace_parsing(false), trace_scanning(false),
      precision(MathOps::MathFactory::Variable<number>("precision", opt.precision)),
      digits(MathOps::MathFactory::Variable<number>("digits", opt.digits))
{
    variables.push_back(precision);
    variables.push_back(digits);

    boost::multiprecision::mpfr_float::default_precision((int) precision->result());
}

int driver::parse_file(const std::string &f)
{
    is_file = true;
    file = f;
    location.initialize(&file);
    scan_file_begin();
    yy::parser parser(*this);
    parser.set_debug_level(trace_parsing);
    int res = parser.parse();
    scan_file_end();

    return res;
}

typedef struct yy_buffer_state *YY_BUFFER_STATE;
extern int yyparse();
extern YY_BUFFER_STATE yy_scan_string(char *str);
extern void yy_delete_buffer(YY_BUFFER_STATE buffer);
extern YY_BUFFER_STATE yy_scan_string(const char *yy_str);

int driver::parse_string(const std::string &line)
{
    is_file = false;
    yy_scan_string(line.c_str());
    scan_begin();
    std::string file({});
    location.initialize(&file);
    yy::parser parser(*this);
    parser.set_debug_level(trace_parsing);
    return parser.parse();
}

void driver::add_var(std::shared_ptr<MathOps::MathOpVariable<number>> variable)
{
    variables.push_back(variable);
}

void driver::make_var(std::string variable)
{
    if (!get_var(variable))
    {
        variables.push_back(MathOps::MathFactory::Variable<number>(variable));
    }
}

void driver::show_variables()
{
    for (auto variable: variables)
    {
        print_result(variable);
    }
}

void driver::clear_variables()
{
    variables.clear();
    variables.push_back(precision);
    variables.push_back(digits);
}

void driver::help()
{
    std::cout << "Syntax:\n"
                 "  Assignments                  : <variable name> = <expression>\n"
                 "                                  Example: c = sqrt(a^2 + b^2)\n"
                 "  Solve for a variable         : solve <variable name>: <expression> == <expession>\n"
                 "                                  Example: solve a: a^2 + b^2 == c^2\n"
                 "  Show all assigned variables  : show\n"
                 "  Clear all assigned variables : clear\n"
                 "  Help                         : help, ?\n"
                 "  Constants                    : %pi, %e\n"
                 "  Math functions               : pow(), log(), sqrt(), sin(), asin(), cos(), acos(), tan(), atan()\n"
                 "\n"
                 "Default variables:\n"
                 "  digits                       : The number of significant digits to display (default: 5)\n"
                 "  precision                    : The number of internal significant digits (default: 50)\n"
                 "\n"
                 "Exit                           : Control-D\n"
                 "\n";
}

void driver::warranty()
{
    std::cout << "Algebla: An equation solving, arbitrary precision calculator\n"
                 "Copyright (C) 2022 Tom Wimmenhove\n"
                 "\n"
                 "This program is free software; you can redistribute it and/or\n"
                 "modify it under the terms of the GNU General Public License\n"
                 "as published by the Free Software Foundation; either version 2\n"
                 "of the License, or (at your option) any later version.\n"
                 "\n"
                 "This program is distributed in the hope that it will be useful,\n"
                 "but WITHOUT ANY WARRANTY; without even the implied warranty of\n"
                 "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n"
                 "GNU General Public License for more details.\n"
                 "\n"
                 "You should have received a copy of the GNU General Public License\n"
                 "along with this program; if not, write to the Free Software\n"
                 "Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.\n"
                 "\n";
}

std::shared_ptr<MathOps::MathOp<number>> driver::solve(std::shared_ptr<MathOps::MathOp<number>> lhs,
                                              std::shared_ptr<MathOps::MathOp<number>> rhs, std::string variable)
{
    auto solve_for = std::static_pointer_cast<MathOps::MathOpVariableBase<number>>(
        lhs->transform(MathOps::MathOpFindVariableTransformer<number>(variable)));

    if (solve_for)
    {
        auto solved = lhs->transform(MathOps::MathOpRearrangeTransformer<number>(solve_for, rhs));

        solve_for->set(solved->result());

        return solved;
    }

    solve_for = std::static_pointer_cast<MathOps::MathOpVariableBase<number>>(
        rhs->transform(MathOps::MathOpFindVariableTransformer<number>(variable)));

    if (!solve_for)
    {
        throw yy::parser::syntax_error(location, "variable " + variable + " appears on neither left or right side");        
    }

    auto solved = rhs->transform(MathOps::MathOpRearrangeTransformer<number>(solve_for, lhs));

    solve_for->set(solved->result());

    return solved;
}

std::shared_ptr<MathOps::MathOp<number>> driver::assign(std::string variable, std::shared_ptr<MathOps::MathOp<number>> op)
{
    auto result = op->result();

    /* Special variables */
    if (variable == precision->get_symbol())
    {
        boost::multiprecision::mpfr_float::default_precision((int) result);
        precision->set((int) result);

        return precision;
    }
    else if (variable == digits->get_symbol())
    {
        digits->set((int) result);

        return digits;
    }

    auto v = get_var(variable);
    if (!v)
    {
        auto new_variable = MathOps::MathFactory::Variable(variable, result);
        variables.push_back(new_variable);

        return new_variable;
    }

    v->set(result);

    return v;
}

std::shared_ptr<MathOps::MathOpVariable<number>> driver::find_var(std::string variable)
{
    auto v = get_var(variable);
    if (!v)
    {
        throw yy::parser::syntax_error(location, "variable " + variable + " has not been declared");
    }

    return v;
}

std::shared_ptr<MathOps::MathOpVariable<number>> driver::get_var(std::string variable)
{
    auto it = std::find_if(variables.begin(), variables.end(),
        [&variable](std::shared_ptr<MathOps::MathOpVariable<number>> v) { return v->get_symbol() == variable; });

    return it == variables.end()
        ? nullptr
        : *it;
}

void driver::print_result(std::shared_ptr<MathOps::MathOp<number>> op)
{
    int int_digits = (int) digits->result();
    int int_precision =(int) precision->result();
    if (int_digits > int_precision)
    {
        std::cerr << "WARNING: Number of digits (digits = " << int_digits << ") exceeds internal precision (precision = " << int_precision << ")\n";
    }

    std::cout << std::setprecision(int_digits);

    if (opt.answer_only)
    {
        std::cout << op->result() << '\n';
        return;
    }

    MathOps::MathOpDefaultFormatter<number> formatter(int_digits);

    std::cout << "  " << op->format(formatter) << " = ";

    std::string uf = useful_fraction<number>(op->result());
    if (uf.empty())
    {
        std::cout << op->result() << '\n';
    }
    else
    {
        std::cout << op->result() << " (~" << uf << ")\n";
    }
}