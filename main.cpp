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
#include <functional>
#include <readline/readline.h>
#include <readline/history.h>

int main(int, char **)
{
    driver drv;
    MathOpDefaultFormatter<number> formatter;

    char *line_buf;
    while ((line_buf = readline("> ")) != nullptr)
    {
        if (strlen(line_buf) > 0)
        {
            add_history(line_buf);
        }

        if (drv.parse_string(line_buf) != 0)
        {
            continue;
        }

        for (auto &exp : drv.get_expressions())
        {
            std::cout << "  " << exp->format(formatter) << " = " << useful_fraction<number>(exp->result()) << '\n';
        }

        free(line_buf);
    }

    return 0;
}
