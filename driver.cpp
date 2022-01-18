#include <iostream>
#include <algorithm>
#include <stdio.h>

#include "driver.h"
#include "parser.h"
#include "mathop/findnamedvaluetransformer.h"
#include "mathop/findexternaltransformer.h"
#include "mathop/rearrangetransformer.h"
#include "mathop/namedvaluecounter.h"
#include "mathop/defaultformatter.h"
#include "usefulfraction.h"

driver::driver(options opt)
    : trace_parsing(false), trace_scanning(false),
      opt(opt),
      precision(MathOps::Factory::CreateVariable<number>("precision", opt.precision)),
      digits(MathOps::Factory::CreateVariable<number>("digits", opt.digits)),
      ans(MathOps::Factory::CreateVariable<number>("ans", 0)),
      variables({precision, digits, ans})
{
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

void driver::add_var(std::shared_ptr<MathOps::Variable<number>> variable)
{
    variables.push_back(variable);
}

void driver::make_var(std::string variable)
{
    if (!get_var(variable))
    {
        variables.push_back(MathOps::Factory::CreateVariable<number>(variable));
    }
}

void driver::show_variables()
{
    for (auto variable: variables)
    {
        std::cout << "  ";

        print_result(variable);
    }

    for (auto& i: lambdas)
    {
        std::cout << "  ";// << i.first << " => ";
        //print_result(i.second);
        print_result(find_identifier(i.first));
    }
}

void driver::clear_variables()
{
    variables.clear();
    lambdas.clear();
    variables.insert(variables.end(), { precision, digits, ans });
}

void driver::help()
{
    std::cout << "Syntax:\n"
                 "  Assignments                  : <variable name> = <expression>\n"
                 "                                  Example: c = sqrt(a^2 + b^2)\n"
                 "  Lambda assignments           : <variable name> => <expression>\n"
                 "                                  Example: c => a + b\n"
                 "  Solve for a variable         : solve <variable name>: <expression> = <expession>\n"
                 "                                  Example: solve a: a^2 + b^2 = c^2\n"
                 "  Delete a vairable or lambda  : <variable name> =\n"
                 "                                  Example: a =\n"
                 "  Show all assigned variables  : :show\n"
                 "  Clear all assigned variables : :clear\n"
                 "  Help                         : :help\n"
                 "  Constants                    : %pi, %e\n"
                 "  Math functions               : pow(), log(), log10(), sqrt(),\n"
                 "                               : sin(), asin(), cos(), acos(), tan(), atan()\n"
                 "                               : sinh(), asinh(), cosh(), acosh(), tanh(), atanh()\n"
                 "\n"
                 "Default variables:\n"
                 "  digits                       : The number of significant digits to display (default: 5)\n"
                 "  precision                    : The number of internal significant digits (default: 50)\n"
                 "\n";
    if (isatty(fileno(stdin)))
    {
        std::cout << "Exit                           : Control-D\n\n";
    }
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
    MathOps::NamedValueCounter<number> counter(variable);
    int left_count = lhs->count(counter);
    rhs->count(counter);
    auto& variables = counter.get_variables();

    if (variables.size() == 0)
    {
        throw yy::parser::syntax_error(location, "variable " + variable + " appears on neither left or right side");        
    }

    if (variables.size() > 1)
    {
        throw yy::parser::syntax_error(location, "variable " + variable + " appears more than once");        
    }

    auto solve_side  = left_count ? lhs : rhs;
    auto result_side = left_count ? rhs : lhs;

    auto solve_variable = variables[0];

    auto solved = solve_side->transform(MathOps::MathOpRearrangeTransformer<number>(solve_variable, result_side));

    solve_variable->set(solved->result());

    return solved;
}

std::shared_ptr<MathOps::MathOp<number>> driver::assign(std::string variable, std::shared_ptr<MathOps::MathOp<number>> op)
{
    for(auto i: lambdas)
    {
        if (i.second->transform(MathOps::FindExternalTransformer<number>(variable)))
        {
            throw yy::parser::syntax_error(location, variable + " is in use by lambda " + i.first + " as a lambda\n");
        }
    }

    lambdas.erase(variable);

    auto result = op->result();

    /* Special variables */
    if (variable == precision->get_symbol())
    {
        if (opt.max_precision > 0 && (int) result > opt.max_precision)
        {
            std::cerr << "Value exceeds maximum precision.\n";

            return precision;
        }

        if ((int) result < digits->result())
        {
            std::cerr << "Value can not be less than the number of visible digits.\n";

            return precision;
        }

        boost::multiprecision::mpfr_float::default_precision((int) result);
        precision->set((int) result);

        return precision;
    }
    else if (variable == digits->get_symbol())
    {
        if ((int) result > precision->result())
        {
            std::cerr << "Value can not be greater than precision.\n";

            return digits;
        }

        digits->set((int) result);

        return digits;
    }

    auto v = get_var(variable);
    if (!v)
    {
        auto new_variable = MathOps::Factory::CreateVariable(variable, result);
        variables.push_back(new_variable);

        return new_variable;
    }

    v->set(result);

    return v;
}

std::shared_ptr<MathOps::MathOp<number>> driver::assign_lambda(std::string variable, std::shared_ptr<MathOps::MathOp<number>> op)
{
    if (variable == ans->get_symbol() ||
        variable == digits->get_symbol() ||
        variable == precision->get_symbol())
    {
        throw yy::parser::syntax_error(location, variable + " is reserved");
    }

    if (op->transform(MathOps::FindNamedValueTransformer<number>(variable)))
    {
        throw yy::parser::syntax_error(location, "Lambda may not reference a variable with the same name");
    }

    if (op->transform(MathOps::FindExternalTransformer<number>(variable)))
    {
        throw yy::parser::syntax_error(location, "Infinite recursion detected");
    }

    for(auto i: lambdas)
    {
        if (i.second->transform(MathOps::FindNamedValueTransformer<number>(variable)))
        {
            throw yy::parser::syntax_error(location, variable + " is in use by lambda " + i.first + " as a variale\n");
        }
    }

    auto it = std::find_if(variables.begin(), variables.end(),
        [&variable](std::shared_ptr<MathOps::Variable<number>> v) { return v->get_symbol() == variable; });

    if (it != variables.end())
    {
        variables.erase(it);
    }

    return lambdas[variable] = op;
}

void driver::remove(std::string name)
{
    /* Check if variable is in use */
    for(auto i: lambdas)
    {
        if (i.second->transform(MathOps::FindNamedValueTransformer<number>(name)))
        {
            throw yy::parser::syntax_error(location, "Variable " + name + " is in use by lambda " + i.first + "\n");
        }

        if (i.second->transform(MathOps::FindExternalTransformer<number>(name)))
        {
            throw yy::parser::syntax_error(location, "Lambda " + name + " is in use by lambda " + i.first + "\n");
        }
    }

    variables.erase(std::remove_if(variables.begin(), variables.end(),
            [&name](auto v) { return v->get_symbol() == name; }),
        variables.end());

    lambdas.erase(name);
}

std::shared_ptr<MathOps::MathOp<number>> driver::find_identifier(std::string variable)
{
    std::shared_ptr<MathOps::MathOp<number>> v = get_var(variable);
    if (v)
    {
        return v;
    }

    v = get_lambda(variable);
    if (!v)
    {
        throw yy::parser::syntax_error(location, variable + " has not been declared");
    }

    return MathOps::Factory::CreateExternal<number>(
        std::function<std::shared_ptr<MathOps::MathOp<number>>()>(std::bind(&driver::get_lambda, this,  variable)),
        variable);
}

std::shared_ptr<MathOps::MathOp<number>> driver::get_lambda(std::string variable)
{
    auto it = lambdas.find(variable);
    return it == lambdas.end()
        ? nullptr
        : it->second;
}

std::shared_ptr<MathOps::Variable<number>> driver::get_var(std::string variable)
{
    auto it = std::find_if(variables.begin(), variables.end(),
        [&variable](std::shared_ptr<MathOps::Variable<number>> v) { return v->get_symbol() == variable; });

    return it == variables.end()
        ? nullptr
        : *it;
}

void driver::result(std::shared_ptr<MathOps::MathOp<number>> op)
{
    std::cout << "  ";

    number result = print_result(op);
    
    ans->set(result);
}

number driver::print_result(std::shared_ptr<MathOps::MathOp<number>> op)
{
    number result = op->result();

    int int_digits = (int) digits->result();
    std::cout << std::setprecision(int_digits);

    if (opt.answer_only)
    {
        std::cout << result << '\n';
        return result;
    }

    std::cout << op->format(MathOps::DefaultFormatter<number>(int_digits)) << " = ";

    std::string uf = useful_fraction<number>(result);
    if (uf.empty())
    {
        std::cout << result << '\n';
    }
    else
    {
        std::cout << result << " (~" << uf << ")\n";
    }

    return result;
}