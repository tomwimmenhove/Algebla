#include "driver.h"
#include "config.h"

#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>

int main(int, char **)
{
    std::cout << "Algebla: An equation solving, arbitrary precision calculator\n"
                 "Copyright (C) 2022 Tom Wimmenhove\n"
                 "\n"
                 "This is free software with ABSOLUTELY NO WARRANTY.\n"
                 "For details type `warranty'. \n"
                 "\n";

    driver drv;

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

        free(line_buf);
    }

    return 0;
}
