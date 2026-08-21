#pragma once

#include <string>
#include <cstdint>

#include <emscripten.h>

#include "shell_base.hpp"
#include "kernel.hpp"

// extern "C" EMSCRIPTEN_KEEPALIVE kernel::InputOutputMode getMode();

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