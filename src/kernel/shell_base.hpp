#pragma once

#include <cstdarg>
#include <cstdio>
#include <string>

namespace kernel
{
    template <typename Derived>
    class IShell
    {
    public:
        static Derived& getInstance()
        {
            static Derived instance;
            return instance;
        }

        void input(const std::string& inputString)
        {
            derived().inputImpl(inputString);
        }



        void output(const std::string& outputString)
        {
            derived().outputImpl(outputString);
        }

        /* элипсис для того чтоб выводить аргументы */
        void output(const char* format, ...)
        {
            char buffer[4096];

            va_list args;
            va_start(args, format);

            std::vsnprintf(buffer, sizeof(buffer), format, args);

            va_end(args);

            derived().outputImpl(std::string(buffer));
        }

    protected:
        IShell() = default;
        ~IShell() = default;

    private:
        Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }

        const Derived& derived() const
        {
            return static_cast<const Derived&>(*this);
        }
    };
} // namespace kernel