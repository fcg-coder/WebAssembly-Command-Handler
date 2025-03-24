#include "main.hpp"

JSStreamBuffer jsStreamBuffer;
std::ostream jsCout(&jsStreamBuffer);

InputOutputHandler* InputOutputHandler::m_instance = nullptr;
InputOutputHandler* IOH = InputOutputHandler::getInstance();
InputOutputMode InputOutputHandler::mode = InputOutputMode::SHELL;
extern "C" EMSCRIPTEN_KEEPALIVE InputOutputMode getMode()
{

    return InputOutputHandler::getInstance()->mode;
}

extern "C" EMSCRIPTEN_KEEPALIVE uint32_t* getScreen()
{
    if (InputOutputHandler::getInstance()->mode == InputOutputMode::SCREEN)
    {
        return Screen::getInstance()->getScreen();
    }
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
        InputOutputHandler::getInstance()->input(_input);
    }
}

extern "C" EMSCRIPTEN_KEEPALIVE void setSize(const int height, const int width)
{
    Screen::getInstance()->setSize(height, width);
}

int main()
{
    std::cout.rdbuf(&jsStreamBuffer);
    return 0;
}
