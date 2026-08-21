// WebASM compilator's lib
#include "web_ioh.hpp"
#include "../graphic_lib/screen.hpp"

/** SHELL  */
extern "C" EMSCRIPTEN_KEEPALIVE void output(const char* text)
{
    EM_ASM({
        const output = document.getElementById('output');
        if (output) {
            output.innerHTML += UTF8ToString($0) + "<br>";  
            output.scrollTop = output.scrollHeight;   
        } }, text);
}

extern "C" EMSCRIPTEN_KEEPALIVE void input(const char* input)
{
    if (! input)
        return;

    Kernel::IOH()->input(input);
}

/** SCREEN */
extern "C" EMSCRIPTEN_KEEPALIVE kernel::InputOutputMode getMode()
{
    return Kernel::getMode();
}
extern "C" EMSCRIPTEN_KEEPALIVE uint32_t* getScreen()
{
    return Kernel::SCREEN().getScreen();
}
extern "C" EMSCRIPTEN_KEEPALIVE void setSize(const int height, const int width)
{
    Kernel::SCREEN().setSize(height, width);
}
