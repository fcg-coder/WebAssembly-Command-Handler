#pragma once
#include "command_handler/command_handler.hpp"
#include <emscripten.h>
#include <iostream>
#include <atomic>
#include <cmath>
#include <cstdio>
#include <emscripten/bind.h>
class inputOutputHandler;
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

// Объявления глобальных переменных
extern JSStreamBuffer jsStreamBuffer;
extern std::ostream jsCout;

class MainWindow
{
protected:
    static int windowHeight; // Height in pixels
    static int windowWidth;  // Width in pixels

public:
    static int getWidth() { return windowWidth; }
    static int getHeight() { return windowHeight; }
};
/**
 * Class handler  input/output from C++ to JS
 * So it is "Screen"
 *
 */
class inputOutputHandler : public MainWindow
{
private:
    static inputOutputHandler* m_instance;

    std::string m_inputString;
    std::string m_outputString;
    inputOutputHandler() {}

public:
    inputOutputHandler(const inputOutputHandler&) = delete;
    inputOutputHandler& operator=(const inputOutputHandler&) = delete;

    static inputOutputHandler* getInstance()
    {
        if (! m_instance)
        {
            m_instance = new inputOutputHandler();
        }
        return m_instance;
    }

    void setSize(int height, int width)
    {
        int scale = 20;
        windowHeight = std::round(static_cast<double>(height) / scale);
        windowWidth = std::round(static_cast<double>(width) / scale);

        char buffer[50];
        std::snprintf(buffer, sizeof(buffer), "Set Height %d, Width %d", windowHeight, windowWidth);
        output(buffer);
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
