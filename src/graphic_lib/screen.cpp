#include "screen.hpp"
#include "../main.hpp"
Screen* Screen::m_instance = nullptr;
Screen* SCREEN = Screen::getInstance();
extern InputOutputHandler* IOH;

int Screen::m_windowHeight = MAX_HEIGHT;
int Screen::m_windowWidth = MAX_WIDTH;
