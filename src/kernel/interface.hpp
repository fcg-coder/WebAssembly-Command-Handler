#pragma once

#include <string>

namespace kernel
{

    enum class InputOutputMode
    {
        SHELL,
        SCREEN
    };

    class IInputOutputHandler
    {
    public:
        virtual ~IInputOutputHandler() = default;

        virtual void input(const std::string& inputString) = 0;
        virtual void output(const std::string& outputString) = 0;

        virtual void setMode(InputOutputMode mode) = 0;
        virtual InputOutputMode getMode() const = 0;
    };

} // namespace kernel