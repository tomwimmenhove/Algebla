#include "config.h"
#include "options.h"

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <iostream>

options::options(int argc, char **argv)
{
    int c;

    while (1)
    {
        static struct option long_options[] =
            {
                {"answer", 0, 0, 'a'},
                {"quiet", 0, 0, 'q'},
                {"digits", 1, 0, 'd'},
                {"precision", 1, 0, 'p'},
                {"help", 0, 0, 'h'},
                {"version", 0, 0, 'v'},
                {0, 0, 0, 0}};
        int option_index = 0;

        c = getopt_long(argc, argv, "aqd:p:hv",
                        long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
        case 'a':
            answer_only = true;
            break;

        case 'q':
            quiet = true;
            break;

        case 'd':
            digits = parse_string(optarg);
            if (digits < 1)
            {
                std::cerr << "Number of significant digits shold be at least 1\n";
                exit(1);
            }
            break;

        case 'p':
            precision = parse_string(optarg);
            if (precision < 1)
            {
                std::cerr << "Number of significant digits shold be at least 1\n";
                exit(1);
            }
            break;

        case 'v':
            print_version();
            exit(0);

        case 'h':
            print_help(argv[0], false);
            exit(0);

        default:
            print_help(argv[0], true);
            exit(1);
        }
    }

    if (optind < argc)
    {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
        {
            filenames.push_back(argv[optind++]);
        }
    }
}

void options::print_help(std::string name, bool error)
{
    auto& out = error ? std::cerr : std::cout;

    out << "usage: " << name << " [options] [file 1] [file 2] ...\n"
        << "  -h, --help          : This help screen\n"
        << "  -a, --answer        : Show result as answer only\n"
        << "  -q, --quiet         : Suppress disclaimer\n"
        << "  -d, --digits    [n] : Set the number of significant digits to display (default: 5)\n"
        << "  -p, --precision [n] : Set the number of internal significant digits (default: 50)\n"
        << "  -v, --version       : This help screen\n";
}

void options::print_version()
{
    std::cout << ALGEBLA_VERSION << '\n';
}

int options::parse_string(const char *s)
{
    char *endptr = nullptr;
    long number = 0;

    errno = 0;

    int i = strtol(s, &endptr, 10);

    int error = errno;
    if (error)
    {
        std::cerr << "Could not parse integer \"" << s << "\": " << strerror(error) << '\n';
        exit(1);
    }

    if (*endptr != '\0')
    {
        std::cerr << "Could not parse integer \"" << s << "\"\n";
        exit(1);
    }

    return i;
}