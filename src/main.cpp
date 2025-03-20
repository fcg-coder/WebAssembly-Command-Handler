#include "main.hpp"
#include "screen.hpp"
#include "command_handler/command_handler.hpp"
#include <iostream>
#include <streambuf>
#include <string>
#include <atomic>

std::atomic<bool> running(true);
inputOutputHandler* inputOutputHandler::m_instance = nullptr;
JSStreamBuffer jsStreamBuffer;
std::ostream jsCout(&jsStreamBuffer);

int MainWindow::windowHeight = 0;
int MainWindow::windowWidth = 0;

#ifndef DEBUG

extern "C" EMSCRIPTEN_KEEPALIVE uint32_t* getScreen()
{

    return Screen::getInstance()->getScreen();
}

extern "C" EMSCRIPTEN_KEEPALIVE void printToJS(const char* text)
{
    EM_ASM({
        const output = document.getElementById('output');
        if (output) {
            output.innerHTML += UTF8ToString($0) + "<br>";  
            output.scrollTop = output.scrollHeight;   
        } }, text);
}

extern "C" EMSCRIPTEN_KEEPALIVE void processInput(const char* input)
{
    if (input != nullptr)
    {
        std::string _input(input);
        inputOutputHandler::getInstance()->input(_input);
    }
}

extern "C" EMSCRIPTEN_KEEPALIVE void setSize(const int height, const int width)
{
    inputOutputHandler::getInstance()->setSize(height, width);
}

#endif

int main()
{
#ifndef DEBUG
    std::cout.rdbuf(&jsStreamBuffer);
#endif

    return 0;
}
