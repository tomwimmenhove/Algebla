#include <iostream>
#include <algorithm>

#include "driver.h"
#include "parser.h"
#include "findvariabetransformer.h"
#include "rearrangetransformer.h"

using namespace std;

driver::driver()
    : trace_parsing(false), trace_scanning(false)
{
}

int driver::parse_file(const std::string &f)
{
    expressions.clear();
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
    expressions.clear();
    yy_scan_string(line.c_str());
    scan_begin();
    std::string file("string input");
    location.initialize(&file);
    yy::parser parser(*this);
    parser.set_debug_level(trace_parsing);
    return parser.parse();
}

void driver::add_var(std::shared_ptr<MathOpVariable<number>> variable)
{
    variables.push_back(variable);
}

void driver::make_var(std::string variable)
{
    if (!get_var(variable))
    {
        variables.push_back(MathFactory::Variable<number>(variable));
    }
}

std::shared_ptr<MathOp<number>> driver::solve(std::shared_ptr<MathOp<number>> lhs,
    std::shared_ptr<MathOp<number>> rhs, std::string variable)
{
    auto solve_for = std::static_pointer_cast<MathOpVariableBase<number>>(
        lhs->transform(MathOpFindVariableTransformer<number>(variable)));
    //auto solved = op->transform(MathOpRearrangeTransformer<number>(v, MathFactory::ConstantValue<number>(result->result())));

    if (solve_for)
    {
        auto solved = lhs->transform(MathOpRearrangeTransformer<number>(solve_for, rhs));

        solve_for->set(solved->result());

        return solved;
    }

    solve_for = std::static_pointer_cast<MathOpVariableBase<number>>(
        rhs->transform(MathOpFindVariableTransformer<number>(variable)));

    if (!solve_for)
    {
        throw yy::parser::syntax_error(location, "variable " + variable + " appears on neither left or right side");        
    }

    auto solved = rhs->transform(MathOpRearrangeTransformer<number>(solve_for, lhs));

    solve_for->set(solved->result());

    return solved;
}

std::shared_ptr<MathOp<number>> driver::assign(std::string variable, std::shared_ptr<MathOp<number>> op)
{
    auto result = op->result();

    auto v = get_var(variable);
    if (!v)
    {
        auto new_variable = MathFactory::Variable(variable, result);
        variables.push_back(new_variable);

        return new_variable;
    }

    v->set(result);

    return v;
}

std::shared_ptr<MathOpVariable<number>> driver::find_var(std::string variable)
{
    auto v = get_var(variable);
    if (!v)
    {
        throw yy::parser::syntax_error(location, "variable " + variable + " has not been declared");
    }

    return v;
}

std::shared_ptr<MathOpVariable<number>> driver::get_var(std::string variable)
{
    auto it = std::find_if(variables.begin(), variables.end(),
        [&variable](std::shared_ptr<MathOpVariable<number>> v) { return v->get_symbol() == variable; });

    return it == variables.end()
        ? nullptr
        : *it;
}

void driver::add_exp(std::shared_ptr<MathOp<number>> exp)
{
    expressions.push_back(exp);
}
