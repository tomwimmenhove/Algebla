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
                {"max", 1, 0, 'm'},
                {"help", 0, 0, 'h'},
                {"version", 0, 0, 'v'},
                {"tex", 0, 0, 't'},
                {"external", 1, 0, 'e'},
                {0, 0, 0, 0}};
        int option_index = 0;

        c = getopt_long(argc, argv, "aqm:d:p:hvte:",
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
            digits = parse_int(optarg);
            if (digits < 1)
            {
                std::cerr << "Number of visible digits should be at least 1\n";
                exit(1);
            }
            break;

        case 'm':
            max_precision = parse_int(optarg);
            if (max_precision < 1)
            {
                std::cerr << "Masximum precision should be at least 1\n";
                exit(1);
            }
            break;

        case 'p':
            precision = parse_int(optarg);
            if (precision < 1)
            {
                std::cerr << "Precision should be at least 1\n";
                exit(1);
            }
            break;

        case 't':
            use_tex = true;
            break;
        
        case 'e':
            external = optarg;
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

    if (max_precision > 0 && precision > max_precision)
    {
        std::cerr << "Precision can not be greater than the maximum precision.\n";
        exit(1);
    }

    if (digits > precision)
    {
        std::cerr << "The number of visible digits can not be greater than the precision.\n";
        exit(1);
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
        << "  -a, --answer        : Show result as answer only\n"
        << "  -d, --digits    [n] : Set the number of visible digits (default: 5)\n"
        << "  -h, --help          : This help screen\n"
        << "  -m, --max [n]       : Set maximum precision\n"
        << "  -p, --precision [n] : Set the number of internal significant digits (default: 50)\n"
        << "  -q, --quiet         : Suppress disclaimer\n"
        << "  -t, --tex           : Use tex formatter\n"
        << "  -e, --external      : Pass result string to external program\n"
        << "  -v, --version       : This help screen\n";
}

void options::print_version()
{
    std::cout << ALGEBLA_VERSION << '\n';
}

int options::parse_int(const char *s)
{
    char *endptr = nullptr;

    errno = 0;

    long i = strtol(s, &endptr, 10);

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

    return (int) i;
}