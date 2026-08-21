#pragma once

#include <iostream>
#include <streambuf>
#include <string>

#include <emscripten/bind.h>
#include <emscripten.h>

#include "../command_handler/command_handler.hpp"
#include "interface.hpp"
#include "kernel.hpp"

extern "C" EMSCRIPTEN_KEEPALIVE kernel::InputOutputMode getMode();

extern "C" EMSCRIPTEN_KEEPALIVE uint32_t* getScreen();

extern "C" EMSCRIPTEN_KEEPALIVE void output(const char* text);

extern "C" EMSCRIPTEN_KEEPALIVE void input(const char* input);

extern "C" EMSCRIPTEN_KEEPALIVE void setSize(int height, int width);

namespace web
{

    class JSStreamBuffer : public std::streambuf
    {
    private:
        std::string buffer;

    protected:
        int overflow(int c) override
        {
            if (c != EOF)
            {
                buffer += static_cast<char>(c);

                if (c == '\n')
                {
                    output(buffer.c_str());
                    buffer.clear();
                }
            }

            return c;
        }

        int sync() override
        {
            if (! buffer.empty())
            {
                output(buffer.c_str());
                buffer.clear();
            }

            return 0;
        }
    };

    extern JSStreamBuffer jsStreamBuffer;
    extern std::ostream jsCout;

    class WebAsmShell : public kernel::IShell
    {

    public:
        static WebAsmShell& getInstance()
        {
            static WebAsmShell instate;
            return instate;
        };

        void input(const std::string& inputString) override
        {
            CommandHandler::getInstance().execute(inputString);
        }

        void output(const std::string& outputString) override
        {
            m_outputString = outputString;

            if (! m_outputString.empty())
            {
                output(m_outputString.c_str());
                m_outputString.clear();
            }
        }

    private:
        std::string m_outputString;

        WebAsmShell() = default;
    };
} // namespace web