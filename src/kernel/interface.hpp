#pragma once

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
            derived().input(inputString);
        }

        void output(const std::string& outputString)
        {
            derived().output(outputString);
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