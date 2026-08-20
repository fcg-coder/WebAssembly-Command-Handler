#pragma once

#include <iostream>
#include <streambuf>
#include <string>

#include <emscripten/bind.h>
#include <emscripten.h>

#include "../command_handler/command_handler.hpp"
#include "interface.hpp"

namespace web
{

    extern "C" EMSCRIPTEN_KEEPALIVE void printToJS(const char* text);

    extern "C" EMSCRIPTEN_KEEPALIVE void processInput(const char* input);

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
                    printToJS(buffer.c_str());
                    buffer.clear();
                }
            }

            return c;
        }

        int sync() override
        {
            if (! buffer.empty())
            {
                printToJS(buffer.c_str());
                buffer.clear();
            }

            return 0;
        }
    };

    extern JSStreamBuffer jsStreamBuffer;
    extern std::ostream jsCout;

    class WebAsmIOH : public kernel::IInputOutputHandler
    {

    public:
        static WebAsmIOH& getInstance()
        {
            static WebAsmIOH instate;
            return instate;
        };

        void input(const std::string& inputString) override
        {
            m_inputString = inputString;
            CommandHandler::getInstance().execute(m_inputString);
            m_inputString.clear();
        }

        void
        output(const std::string& outputString) override
        {
            m_outputString = outputString;

            if (! m_outputString.empty())
            {
                printToJS(m_outputString.c_str());
                m_outputString.clear();
            }
        }

        void setMode(kernel::InputOutputMode mode) override
        {
            m_mode = mode;
        }

        kernel::InputOutputMode getMode() const override
        {
            return m_mode;
        }

    private:
        std::string m_inputString;
        std::string m_outputString;

        WebAsmIOH() = default;
        kernel::InputOutputMode m_mode =
            kernel::InputOutputMode::SHELL;
    };
} // namespace web