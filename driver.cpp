#include <iostream>

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

void driver::make_var(std::string variable)
{
    auto it = variables.find(variable);
    if (it == variables.end())
    {
        variables[variable] = MathFactory::Variable<number>(variable);
    }
}

std::shared_ptr<MathOp<number>> driver::solve(std::shared_ptr<MathOp<number>> op, std::string variable, number result)
{
    auto v = op->transform(MathOpFindVariableTransformer<number>(variable));

    return op->transform(MathOpRearrangeTransformer<number>(v, MathFactory::ConstantValue<number>(result)));
}

std::shared_ptr<MathOp<number>> driver::assign(std::string variable, std::shared_ptr<MathOp<number>> op)
{
    auto result = op->result();

    auto it = variables.find(variable);
    if (it == variables.end())
    {
        return variables[variable] = MathFactory::Variable(variable, result);
    }

    it->second->set(result);

    return it->second;
}

#include <exception>

std::shared_ptr<MathOpVariableBase<number>> driver::find_var(std::string variable)
{
    auto it = variables.find(variable);
    if (it == variables.end())
    {
        //return variables[variable] = MathFactory::Variable(variable, std::numeric_limits<number>::quiet_NaN());
        throw yy::parser::syntax_error(location, "variable " + variable + " has not been declared");
    }

    return it->second;
}

void driver::add_exp(std::shared_ptr<MathOp<number>> exp)
{
    expressions.push_back(exp);
}
