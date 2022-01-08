#include "defaulthelper.h"
#include "algeblah.h"
#include "findvariabetransformer.h"
#include "replacetransformer.h"
#include "removenooptransformer.h"
#include "rearrangetransformer.h"
#include "defaultformatter.h"
#include "driver.h"
#include "usefulfraction.h"

#include "config.h"

#include <iostream>
#include <iomanip>
#include <functional>
#include <readline/readline.h>
#include <readline/history.h>

#include <mpreal.h>

int main(int, char **)
{
    driver drv;

    auto precision = MathFactory::Variable<number>("precision", 50);
    auto digits = MathFactory::Variable<number>("digits", 5);

    drv.add_var(precision);
    drv.add_var(digits);

    char *line_buf;
    while ((line_buf = readline("> ")) != nullptr)
    {
        if (strlen(line_buf) > 0)
        {
            add_history(line_buf);
        }

        int int_prec = (int) precision->result();
        precision->set(int_prec);
        mpfr::mpreal::set_default_prec(mpfr::digits2bits(int_prec));

        if (drv.parse_string(line_buf) != 0)
        {
            continue;
        }

        int int_digits = (int) digits->result();
        digits->set(int_digits);
        std::cout << std::setprecision(int_digits);
        MathOpDefaultFormatter<number> formatter(int_digits);

        if (int_digits > int_prec)
        {
            std::cerr << "WARNING: Number of digits (digits=" << int_digits << ") exceeds internal precision (precision" << int_prec << ")\n";
        }

        for (auto &exp : drv.get_expressions())
        {
            std::cout << "  " << exp->format(formatter) << " = ";

            std::string uf = useful_fraction<number>(exp->result());
            if (uf.empty())
            {
                std::cout << exp->result() << '\n';
            }
            else
            {
                std::cout << exp->result() << " (~= " << uf << " )\n";                
            }
        }

        free(line_buf);
    }

    return 0;
}
