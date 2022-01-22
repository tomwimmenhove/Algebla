#include "driver.h"
#include "options.h"
#include "config.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <getopt.h>
#include <stdio.h>
#include <signal.h>
#include <setjmp.h>

static bool in_terminal = isatty(fileno(stdin));

jmp_buf jump_buffer;

bool get_input(std::string& str)
{
    if (!in_terminal)
    {
	    if(std::getline(std::cin, str))
        {
            return true;
        }

        return false;
    }
    
    /* Ignore Control-C */
    while(sigsetjmp(jump_buffer, 1))
    {
        std::cout << '\n';
    }

    char *line_buf = readline("> ");
    if (!line_buf)
    {
        return false;
    }

    str = std::string(line_buf);

    free(line_buf);

    return true;
}

void signal_handler(int signum)
{
   siglongjmp(jump_buffer, 1);
}

int main(int argc, char** argv)
{
    options opt(argc, argv);

    if (!opt.quiet && opt.filenames.empty() && isatty(fileno(stdin)))
    {
        std::cout << "Algebla: An equation solving, arbitrary precision calculator\n"
                     "Copyright (C) 2022 Tom Wimmenhove\n"
                     "\n"
                     "This is free software with ABSOLUTELY NO WARRANTY.\n"
                     "For details, type `:warranty'. For help, type ':help' \n"
                     "\n";
    }

    if (!opt.filenames.empty())
    {
        for (auto filename : opt.filenames)
        {
            driver drv(opt);
            int r = drv.parse_file(filename.c_str());
            if (r != 0)
            {
                return r;
            }
        }

        return 0;
    }

    std::string last_line;
    std::string history_path;
    if (in_terminal)
    {
        signal(SIGINT, signal_handler);

        /* Read history */
        history_path = std::string(getpwuid(getuid())->pw_dir) + "/.algebla_history";
        std::ifstream history_file(history_path);
        std::string history_line;
        while (std::getline(history_file, history_line))
        {
            last_line = history_line;
            add_history(history_line.c_str());
        }
    }

    driver drv(opt);
    std::string line;
    rl_bind_key('\t', rl_insert);
    while (get_input(line))
    {
        if (in_terminal && !line.empty() && line != last_line)
        {
            add_history(line.c_str());
            std::ofstream history_file(history_path, std::ios_base::app | std::ios_base::out);
            history_file << line << '\n';

            last_line = line;
        }

        if (drv.parse_string(line) != 0)
        {
            continue;
        }
    }

    return 0;
}
