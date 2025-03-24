#pragma once

#include "command_handler/command_handler.hpp"
#include "graphic_lib/screen.hpp"
// cpp lib
#include <iostream>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <streambuf>
#include <string>

// WebASM compilator's lib
#include <emscripten.h>
#include <emscripten/bind.h>

class InputOutputHandler;
class CommandHandler;

extern "C" EMSCRIPTEN_KEEPALIVE void printToJS(const char* text);
extern "C" EMSCRIPTEN_KEEPALIVE void processInput(const char* input);

class JSStreamBuffer : public std::streambuf
{
private:
    std::string buffer;

protected:
    virtual int overflow(int c) override
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

    virtual int sync() override
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

enum class InputOutputMode
{
    SHELL,
    SCREEN,
};
/**
 * Class handler  input/output from C++ to JS
 * So it is "Screen"
 *
 */
class InputOutputHandler
{
private:
    static InputOutputHandler* m_instance;

    std::string m_inputString;
    std::string m_outputString;
    InputOutputHandler() {}

public:
    static InputOutputMode mode;

    InputOutputHandler(const InputOutputHandler&) = delete;
    InputOutputHandler& operator=(const InputOutputHandler&) = delete;

    static InputOutputHandler* getInstance()
    {
        if (! m_instance)
        {
            mode = InputOutputMode::SHELL;
            m_instance = new InputOutputHandler();
        }
        return m_instance;
    }

    void handler()
    {
        if (! m_outputString.empty())
        {
            output(m_outputString);
            m_outputString = "";
        }
        if (! m_inputString.empty())
        {
            CommandHandler::getInstance()->setCommand(m_inputString);
            m_inputString = "";
        }
    }
    void output(const std::string& outputString)
    {
        printToJS(outputString.c_str());
    }
    void input(const std::string& inputString)
    {
        m_inputString = inputString;
        handler();
    }
};
