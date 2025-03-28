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
    char buffer[50];
    std::snprintf(buffer, sizeof(buffer), "Set screen:  Width %d pix, Height %d pix", m_windowWidth, m_windowHeight);
    IOH->output(buffer);
}