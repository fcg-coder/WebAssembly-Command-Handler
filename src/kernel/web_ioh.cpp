// WebASM compilator's lib
#include "web_ioh.hpp"
#include "kernel.hpp"

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
    // Kernel::execute(inputString);
}

/** SCREEN */
extern "C" EMSCRIPTEN_KEEPALIVE kernel::InputOutputMode getMode()
{
    return Kernel::getMode();
}

extern "C" EMSCRIPTEN_KEEPALIVE uint32_t* getScreen()
{
    return Kernel::SCREEN()->getScreen();
}
extern "C" EMSCRIPTEN_KEEPALIVE void setSize(const int height, const int width)
{
    Kernel::SCREEN()->setSize(height, width);
}

namespace kernel
{
    void WebAsmShell::inputImpl(const std::string& inputString)
    {
        Kernel::executeCmd(inputString);
    }

    void WebAsmShell::outputImpl(const std::string& outputString)
    {
        m_outputString = outputString;

        if (! m_outputString.empty())
        {
            ::output(m_outputString.c_str());
            m_outputString.clear();
        }
    }
} // namespace kernel