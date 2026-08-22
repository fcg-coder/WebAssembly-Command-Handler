#pragma once

#include <string>
#include <cstdint>

#include <emscripten.h>
#include "kernel.hpp"

#include "shell_base.hpp"

namespace kernel
{
    enum class InputOutputMode;
}
extern "C" EMSCRIPTEN_KEEPALIVE kernel::InputOutputMode getMode();

extern "C" EMSCRIPTEN_KEEPALIVE uint32_t* getScreen();

extern "C" EMSCRIPTEN_KEEPALIVE void output(const char* text);

extern "C" EMSCRIPTEN_KEEPALIVE void input(const char* input);

extern "C" EMSCRIPTEN_KEEPALIVE void setSize(int height, int width);

namespace kernel
{
    class WebAsmShell : public kernel::IShell<WebAsmShell>
    {
        friend class kernel::IShell<WebAsmShell>;

    public:
        using IShell<WebAsmShell>::output;

        void inputImpl(const std::string& inputString);
        void outputImpl(const std::string& outputString);

    private:
        WebAsmShell() = default;
        std::string m_outputString;
    };
} // namespace kernel