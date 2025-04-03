#include "screen.hpp"
#include "../main.hpp"
Screen* Screen::m_instance = nullptr;
Screen* SCREEN = Screen::getInstance();
extern InputOutputHandler* IOH;

int Screen::m_windowHeight = MAX_HEIGHT;
int Screen::m_windowWidth = MAX_WIDTH;

void Screen::setSize(int h, int w)
{
    m_windowHeight = h;
    m_windowWidth = w;
    char buffer[60];
#if defined(SIMD) && defined(__wasm_simd128__)
    std::snprintf(buffer, sizeof(buffer), "Set screen:  Width %d pix, Height %d pix with SIMD", m_windowWidth, m_windowHeight);
#else
    std::snprintf(buffer, sizeof(buffer), "Set screen:  Width %d pix, Height %d pix", m_windowWidth, m_windowHeight);
#endif

    IOH->output(buffer);
}

void Screen::addObject(const std::string& key, ShapeBase* shape)
{
    std::string _output = "Add object " + key + " to the scene";
    IOH->output(_output);
    m_scene[key] = shape;
}

void Screen::initializeScene()
{
    addObject("coordinate_system", new CoordinateSystem());
    addObject("cube", new Cube());
}
