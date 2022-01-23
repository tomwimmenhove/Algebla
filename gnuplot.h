#ifndef GNUPLOT_H
#define GNUPLOT_H

#include "mathop/defaultformatter.h"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <errno.h>

template<typename T>
class GnuPlot
{
public:
    void open()
    {
        pipe = popen("gnuplot", "w");
        if (!pipe)
        {
            std::cerr << "Failed to launch gnuplot: " << strerror(errno) << '\n';
        }
    }

    inline void close() { pclose(pipe); }
    inline bool is_open() { return pipe != nullptr; }

    void plot(std::shared_ptr<MathOps::MathOp<T>> equation,
        std::shared_ptr<MathOps::Variable<T>> x, T from, T to, T step, int digits)
    {
        std::stringstream ss;

        std::string title = escape(equation->format(MathOps::DefaultFormatter<T>(digits)));

        std::string x_name = x->get_symbol();
        ss << "set xlabel \"" << x_name << "\"\n";
        ss << "set ylabel \"f(" << x_name << ")\"\n";
        ss << "plot '-' using 1:2 title \"" << title << "\" with lines\n";

        ss << std::setprecision(digits);

        for (T i = from; i < to; i += step)
        {
            x->set(i);
            ss << i << ' ' << equation->result() << '\n';
        }

        ss << "EOF\n";

        fprintf(pipe, "%s", ss.str().c_str());
        fflush(pipe);
    }

private:
    FILE* pipe = nullptr;

    inline std::string escape(std::string s)
    {
        size_t pos = 0;
        while ((pos = s.find('^', pos)) != std::string::npos)
        {
            s.replace(pos, 1, "\\\\^");
            pos += 3;
        }

        return s;
    }
};

#endif /* GNUPLOT_H */