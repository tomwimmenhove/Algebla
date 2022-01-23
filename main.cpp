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
static jmp_buf jump_buffer;
static bool can_jump = false;
static int quit = 0;

void signal_handler(int signum)
{
    if (++quit == 3)
    {
        exit(signum);
    }

    if (can_jump)
    {
        siglongjmp(jump_buffer, 1);
    }
}

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

    can_jump = true;
    char *line_buf = readline("> ");
    can_jump = false;
    quit = 0;
    if (!line_buf)
    {
        return false;
    }

    str = std::string(line_buf);

    free(line_buf);

    return true;
}

// template<typename T>
// inline static T gcd(T a, T b)
// {
//     T r = a % b;
//     return r == 0 ? b : gcd(b, r);
// }

#include <cmath>
#include <numeric>

template<typename F, typename I>
struct IntFraction
{
    IntFraction() : IntFraction(0, 1) { }

    IntFraction(I numerator, I denominator)
     : numerator(numerator), denominator(denominator)
    { }

    I numerator;
    I denominator;

    F result() const { return (F) numerator / denominator; }
    bool is_valid () const { return denominator != 0; }

    IntFraction<F, I> simplified() const
    {
        I gcd = std::gcd<I>(numerator, denominator);
        
        return IntFraction<F, I>(numerator / gcd, denominator / gcd);
    }

    static IntFraction<F, I> invalid() { return IntFraction<F, I>(0, 0); }

    static IntFraction<F, I> find(F value, F max_error, int iters)
    {
        return find(value, value, max_error, iters, [](auto f) { return f.result(); });
    }

    template<typename Func>
    static IntFraction<F, I> find(F value, F test_value, F max_error, int iters, Func fn)
    {
        std::cout << "Value     : " << value << '\n';
        std::cout << "Test value: " << test_value << '\n';

        F c = ceil(value);
        F f = floor(value);

        /* Under or overflow? */
        if (c < std::numeric_limits<I>::min() || c > std::numeric_limits<I>::max() ||
            f < std::numeric_limits<I>::min() || f > std::numeric_limits<I>::max())
        {
            return IntFraction<F, I>::invalid();
        }

        I upper_numerator = (I)c;
        I lower_numerator = (I)f;

        if (upper_numerator == lower_numerator)
        {
            return IntFraction<F, I>(lower_numerator, 1);
        }

        IntFraction<F, I> lower(lower_numerator, 1);
        IntFraction<F, I> upper(upper_numerator, 1);

        while (iters--)
        {
            /* Under or overflow? */
            if (((upper.numerator > 0) && (lower.numerator > std::numeric_limits<I>::max() - upper.numerator)) ||
                ((upper.numerator < 0) && (lower.numerator < std::numeric_limits<I>::min() - upper.numerator)) ||
                ((upper.denominator > 0) && (lower.denominator > std::numeric_limits<I>::max() - upper.denominator)) ||
                ((upper.denominator < 0) && (lower.denominator < std::numeric_limits<I>::min() - upper.denominator)))
            {
                return IntFraction<F, I>::invalid();
            }

            IntFraction<F, I> fraction(lower.numerator + upper.numerator, lower.denominator + upper.denominator);
            F test = fn(fraction);

            if (MathOps::abs(test - test_value) <= max_error)
            {
                return fraction;//.simplified();
            }

            if (test_value > test)
            {
                lower = fraction;
            }
            else
            {
                upper = fraction;
            }
        }

        return IntFraction<F, I>::invalid();
    }
};

#include "mathop/constants.h"
#include "mathop/rearrangetransformer.h"
#include "mathop/removenooptransformer.h"
#include "mathop/replacetransformer.h"
#include "mathop/defaultformatter.h"

template<typename T>
std::shared_ptr<MathOps::MathOp<T>> find_approximation(std::shared_ptr<MathOps::MathOp<T>> equation,
    std::shared_ptr<MathOps::Value<T>> numerator,
    std::shared_ptr<MathOps::Value<T>> denominator,
    T value)
{
    denominator->set(1);

    auto fn = [&](auto fraction)
    {
        numerator->set(fraction.numerator);
        denominator->set(fraction.denominator);

        return equation->result();
    };

    auto result = MathOps::ConstantValue<T>::create(value);
    auto solved = equation->transform(MathOps::MathOpRearrangeTransformer<T>(numerator, result));

    auto fraction = IntFraction<T, long long int>::find(solved->result(), value, .0001, 20000, fn);

    if (fraction.is_valid())
    {
        return equation->transform(MathOps::ReplaceTransformer<T>(numerator,
                            MathOps::ConstantValue<T>::create(fraction.numerator)))
                       ->transform(MathOps::ReplaceTransformer<T>(denominator,
                            MathOps::ConstantValue<T>::create(fraction.denominator)))
                       ->transform(MathOps::MathOpRemoveNoOpTransformer<T>());
    }

    return nullptr;
}

int main(int argc, char** argv)
{
    auto numerator = MathOps::ValueVariable<number>::create("numerator", 1.0);
    auto denominator = MathOps::ValueVariable<number>::create("denominator", 1.0);
    const auto e = MathOps::Constants::e<number>();

    //auto eq = numerator / denominator;
    auto eq = pow<number>(e, numerator) / denominator;

    // (e ^ 1) / 2 == (e ^ numerator) / denominator == 1.3591

    auto input = pow<number>(e, MathOps::ConstantValue<number>::create(2)) / 
         MathOps::ConstantValue<number>::create(2);

    auto result = input->result();

    int precision = 5;
    auto approximation = find_approximation<number>(eq, numerator, denominator, result);    

    if (approximation)
    {
        std::cout << approximation->format(MathOps::DefaultFormatter<number>(precision)) << '\n';
    }

    //return 0;
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
