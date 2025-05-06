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
    // if ((InputOutputHandler::getInstance()->mode == InputOutputMode::SCREEN) || (InputOutputHandler::getInstance()->mode == InputOutputMode::BOTH))
    // {
    return Screen::getInstance()->getScreen();
    // }
    // return 0;
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

extern "C" EMSCRIPTEN_KEEPALIVE void pressButton(const char* _input)
{
    std::string input(_input);

    if (input == "up")
    {
        Menu::getCurrentInstance()->moveUp();
    }
    else if (input == "down")
    {
        Menu::getCurrentInstance()->moveDown();
    }

    else if (input == "enter")
    {
        Menu::getCurrentInstance()->pressEnter();
    }

    else if (input == "escape")
    {
        Menu::destroyInstance();
    }
}
int main()
{
    std::cout.rdbuf(&jsStreamBuffer);
    return 0;
}
