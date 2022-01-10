#ifndef OPTIONS_H
#define OPTIONS_H

#include <vector>
#include <string>

struct options
{
    int digits = 5;
    int precision = 50;
    bool quiet = false;
    bool answer_only = false;
    int max_precision = -1;
    std::vector<std::string> filenames;

    options(int argc, char** argv);

private:
    void print_help(std::string name, bool error);
    void print_version();
    int parse_int(const char* s);
};

#endif /* OPTIONS_H */