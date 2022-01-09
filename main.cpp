#include "driver.h"
#include "config.h"

#include <iostream>
#include <fstream>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>

int main(int, char **)
{
    std::cout << "Algebla: An equation solving, arbitrary precision calculator\n"
                 "Copyright (C) 2022 Tom Wimmenhove\n"
                 "\n"
                 "This is free software with ABSOLUTELY NO WARRANTY.\n"
                 "For details type `warranty'. \n"
                 "\n";

    driver drv;

    /* Read history */
    std::string last_line;
    const std::string history_path = std::string(getpwuid(getuid())->pw_dir) + "/.algebla_history";//"~/.algebla_history";
    std::ifstream history_file(history_path);
    std::string history_line;
    while (std::getline(history_file, history_line))
    {
        last_line = history_line;
        add_history(history_line.c_str());
    }

    char *line_buf;
    rl_bind_key('\t', rl_insert);
    while ((line_buf = readline("> ")) != nullptr)
    {
        std::string line(line_buf);

        if (!line.empty())
        {
            if (line != last_line)
            {
                add_history(line_buf);
                std::ofstream history_file(history_path, std::ios_base::app | std::ios_base::out);
                history_file << line << '\n';
            }

            last_line = line;
        }

        if (drv.parse_string(line) != 0)
        {
            continue;
        }

        free(line_buf);
    }

    return 0;
}
