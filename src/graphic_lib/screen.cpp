#include "screen.hpp"
#include "../main.hpp"
Screen* Screen::m_instance = nullptr;
extern InputOutputHandler* IOH;

int Screen::m_windowHeight = MAX_HEIGH;
int Screen::m_windowWidth = MAX_WIDTH;

void Screen::setSize(int h, int w)
{
    m_windowHeight = h;
    m_windowWidth = w;
    char buffer[50];
    std::snprintf(buffer, sizeof(buffer), "Set Height %d, Width %d", m_windowHeight, m_windowWidth);
    IOH->output(buffer);
}
