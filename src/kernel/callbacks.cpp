// WebASM compilator's lib
#include "callbacks.hpp"
#include "../graphic_lib/screen.hpp"
#include "../command_handler/menu/menu.hpp"

extern "C" EMSCRIPTEN_KEEPALIVE
    kernel::InputOutputMode
    getMode()
{

    auto* ioh = Kernel::IOH();

    if (! ioh)
    {

        return kernel::InputOutputMode::SHELL;
    }

    return ioh->getMode();
}
extern "C" EMSCRIPTEN_KEEPALIVE uint32_t* getScreen()
{
    auto screenPtr = Screen::getInstance().getScreen();
    return screenPtr;
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

    if (! input)
        return;

    Kernel::IOH()->input(input);
}
extern "C" EMSCRIPTEN_KEEPALIVE void setSize(const int height, const int width)
{
    Screen::getInstance().setSize(height, width);
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

extern "C" EMSCRIPTEN_KEEPALIVE void initScreen()
{
    printToJS("initScreen called");
}
