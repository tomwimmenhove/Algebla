#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>

#include "driver.h"
#include "parser.h"
#include "mathop/containercounter.h"
#include "mathop/rearrangetransformer.h"
#include "mathop/expandtransformer.h"
#include "mathop/namedvaluecounter.h"
#include "mathop/defaultformatter.h"
#include "mathop/constants.h"
#include "usefulfraction.h"

driver::driver(options opt)
    : trace_parsing(false), trace_scanning(false),
      opt(opt),
#ifdef ARBIT_PREC
      precision(MathOps::Variable<number>::create("precision", opt.precision)),
#endif
      digits(MathOps::Variable<number>::create("digits", opt.digits)),
      ans(MathOps::Variable<number>::create("ans", 0)),
#ifdef ARBIT_PREC
      variables({precision, digits, ans})
#else
      variables({digits, ans})
#endif
{
#ifdef ARBIT_PREC
    boost::multiprecision::mpfr_float::default_precision((int) precision->result());
#endif
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

void driver::make_var(std::string variable)
{
    if (!get_var(variable))
    {
        variables.push_back(MathOps::Variable<number>::create(variable));
    }
}

void driver::show_variables()
{
    for (auto variable: variables)
    {
        print_result(variable);
    }

    for (auto lambda: lambdas)
    {
        print_result(lambda);
    }
}

void driver::clear_variables()
{
    variables.clear();
    lambdas.clear();
#ifdef ARBIT_PREC
    variables.insert(variables.end(), { precision, digits, ans });
#else
    variables.insert(variables.end(), { digits, ans });
#endif
}

void driver::help()
{
    std::cout << "Syntax:\n"
                 "  Assignments                  : <variable name> = <expression>\n"
                 "                                  Example: c = sqrt(a^2 + b^2)\n"
                 "  Lambda assignments           : <lambda name> => <expression>\n"
                 "                                  Example: c => a + b\n"
                 "  Expanding a lambda:          : expand(<lambda name>)"
                 "                                  Example: c => expand(c)\n"
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
                 "  ans                          : The result of the last calculation\n"
                 "  digits                       : The number of significant digits to display (default: 5)\n"
#ifdef ARBIT_PREC
                 "  precision                    : The number of internal significant digits (default: 50)\n"
#endif
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

    // XXX: Should we always expand the entire tree before solving?
    auto solve_side  = (left_count ? lhs : rhs)->transform(MathOps::ExpandTransformer<number>());
    auto result_side = (left_count ? rhs : lhs)->transform(MathOps::ExpandTransformer<number>());

    auto solve_variable = variables[0];

    auto solved = solve_side->transform(MathOps::MathOpRearrangeTransformer<number>(solve_variable, result_side));

    solve_variable->set(solved->result());

    return solved;
}

std::shared_ptr<MathOps::MathOp<number>> driver::assign(std::string variable, std::shared_ptr<MathOps::MathOp<number>> op)
{
    for(auto lambda: lambdas)
    {
        if (MathOps::ContainerCounter<number>::FindFirst(lambda->get_inner(), variable))
        {
            throw yy::parser::syntax_error(location, variable + " is in use by lambda " + lambda->get_name() + " as a lambda\n");
        }
    }

    auto it = std::find_if(lambdas.begin(), lambdas.end(),
        [&variable](std::shared_ptr<MathOps::Container<number>> l) { return l->get_name() == variable; });

    if (it != lambdas.end())
    {
        lambdas.erase(it);
    }

    auto result = op->result();

    /* Special variables */
    if (variable == digits->get_symbol())
    {
#ifdef ARBIT_PREC
        if ((int) result > precision->result())
        {
            std::cerr << "Value can not be greater than precision.\n";

            return digits;
        }
#endif

        digits->set((int) result);

        return digits;
    }
#ifdef ARBIT_PREC
    else if (variable == precision->get_symbol())
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
#endif

    auto v = get_var(variable);
    if (!v)
    {
        v = MathOps::Variable<number>::create(variable, result);
        variables.push_back(v);

        return v;
    }

    v->set(result);

    return v;
}

std::shared_ptr<MathOps::MathOp<number>> driver::assign_lambda(std::string variable, std::shared_ptr<MathOps::MathOp<number>> op)
{
    check_reserved(variable);

    if (MathOps::NamedValueCounter<number>::FindFirst(op, variable))
    {
        throw yy::parser::syntax_error(location, "Lambda may not reference a variable with the same name");
    }

    if (MathOps::ContainerCounter<number>::FindFirst(op, variable))
    {
        throw yy::parser::syntax_error(location, "Infinite recursion detected");
    }

    for(auto lambda: lambdas)
    {
        if (MathOps::NamedValueCounter<number>::FindFirst(lambda, variable))
        {
            throw yy::parser::syntax_error(location, variable + " is in use by lambda " + lambda->get_name() + " as a variale\n");
        }
    }

    auto it = std::find_if(variables.begin(), variables.end(),
        [&variable](std::shared_ptr<MathOps::Variable<number>> v) { return v->get_symbol() == variable; });

    if (it != variables.end())
    {
        variables.erase(it);
    }

    auto l = get_lambda(variable);
    if (!l)
    {
        l = MathOps::Container<number>::create(op, variable);

        lambdas.push_back(l);

        return l;
    }

    l->set_inner(op);

    return l;
}

void driver::remove(std::string name)
{
    check_reserved(name);

    /* Check if variable is in use */
    for(auto lambda: lambdas)
    {
        if (MathOps::NamedValueCounter<number>::FindFirst(lambda, name))
        {
            throw yy::parser::syntax_error(location, "Variable " + name + " is in use by lambda " + lambda->get_name() + "\n");
        }

        if (MathOps::ContainerCounter<number>::FindFirst(lambda->get_inner(), name))
        {
            throw yy::parser::syntax_error(location, "Lambda " + name + " is in use by lambda " + lambda->get_name() + "\n");
        }
    }

    variables.erase(std::remove_if(variables.begin(), variables.end(),
            [&name](auto v) { return v->get_symbol() == name; }),
        variables.end());

    lambdas.erase(std::remove_if(lambdas.begin(), lambdas.end(),
            [&name](auto l) { return l->get_name() == name; }),
        lambdas.end());
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

    return v;
}

std::shared_ptr<MathOps::Container<number>> driver::get_lambda(std::string variable)
{
    auto it = std::find_if(lambdas.begin(), lambdas.end(),
        [&variable](std::shared_ptr<MathOps::Container<number>> v) { return v->get_name() == variable; });

    return it == lambdas.end()
        ? nullptr
        : *it;
}

std::shared_ptr<MathOps::Variable<number>> driver::get_var(std::string variable)
{
    auto it = std::find_if(variables.begin(), variables.end(),
        [&variable](std::shared_ptr<MathOps::Variable<number>> v) { return v->get_symbol() == variable; });

    return it == variables.end()
        ? nullptr
        : *it;
}

void driver::check_reserved(std::string variable)
{
    if (variable == ans->get_symbol()
        || variable == digits->get_symbol()
#ifdef ARBIT_PREC
        || variable == precision->get_symbol())
#else
    )
#endif
    {
        throw yy::parser::syntax_error(location, variable + " is reserved");
    }
}

std::shared_ptr<MathOps::MathOp<number>> driver::function(std::string func, std::shared_ptr<MathOps::MathOp<number>> op)
{
    if (func == "sqrt")   return MathOps::sqrt(op);
    if (func == "log")    return MathOps::log(op);
    if (func == "log10")  return MathOps::log10(op);
    if (func == "sin")    return MathOps::sin(op);
    if (func == "cos")    return MathOps::cos(op);
    if (func == "tan")    return MathOps::tan(op);
    if (func == "asin")   return MathOps::asin(op);
    if (func == "acos")   return MathOps::acos(op);
    if (func == "atan")   return MathOps::atan(op);
    if (func == "sinh")   return MathOps::sinh(op);
    if (func == "cosh")   return MathOps::cosh(op);
    if (func == "tanh")   return MathOps::tanh(op);
    if (func == "asinh")  return MathOps::asinh(op);
    if (func == "acosh")  return MathOps::acosh(op);
    if (func == "atanh")  return MathOps::atanh(op);
    if (func == "expand") return op->transform(MathOps::ExpandTransformer<number>());

    throw yy::parser::syntax_error(location, "Uknown function: " + func);
}

std::shared_ptr<MathOps::MathOp<number>> driver::get_constant(std::string id)
{
    if (id == "e")  return MathOps::Constants::e<number>();
    if (id == "pi") return MathOps::Constants::pi<number>();
    else throw yy::parser::syntax_error(location, "Uknown constant: " + id);
}

void driver::command(std::string cmd)
{
    if      (cmd == "help")     help();
    else if (cmd == "warranty") warranty();
    else if (cmd == "show")     show_variables();
    else if (cmd == "clear")    clear_variables();
    else throw yy::parser::syntax_error(location, "Uknown command: " + cmd);
}

void driver::result(std::shared_ptr<MathOps::MathOp<number>> op)
{
    number result = print_result(op);
    
    ans->set(result);
}

number driver::print_result(std::shared_ptr<MathOps::MathOp<number>> op)
{
    number result = op->result();

    int int_digits = (int) digits->result();
    std::cout << std::setprecision(int_digits);
    std::cout << "  ";

    if (opt.answer_only)
    {
        std::cout << result << '\n';
        return result;
    }

    /* Expand containers? */
    auto container = MathOps::ContainerCounter<number>::FindFirst(op, "");
    if (container == op)
    {
        std::cout << op->format(MathOps::DefaultFormatter<number>(int_digits, false)) << " => ";
        if (MathOps::ContainerCounter<number>::FindFirst(container->get_inner(), ""))
        {
            std::cout << container->get_inner()->format(MathOps::DefaultFormatter<number>(int_digits, false)) << " = ";
        }
    }
    else if (container)
    {
        std::cout << op->format(MathOps::DefaultFormatter<number>(int_digits, false)) << " = ";
    }

    std::cout << op->format(MathOps::DefaultFormatter<number>(int_digits, true)) << " = ";

    std::string uf = useful_fraction<number>(result, int_digits);
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