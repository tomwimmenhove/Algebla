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

int main(int, char **)
{
    driver drv;
    MathOpDefaultFormatter<number> formatter;

    auto precision = MathFactory::Variable<number>("precision", 5);

    drv.add_var(precision);

    char *line_buf;
    while ((line_buf = readline("> ")) != nullptr)
    {
        if (strlen(line_buf) > 0)
        {
            add_history(line_buf);
        }

        if (drv.parse_string(std::string(line_buf) + ";") != 0)
        {
            continue;
        }

        int int_pred = (int) precision->result();
        precision->set(int_pred);
        std::cout << std::setprecision(int_pred);

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
