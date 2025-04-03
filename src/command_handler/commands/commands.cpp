#include "commands.hpp"
#include "../../graphic_lib/screen.hpp"

extern InputOutputHandler* IOH;

void CommandHelp::execute()
{
    IOH->output("Help handler");
}

void CommandScreen::execute()
{
    IOH->mode = InputOutputMode::SCREEN;
}

void CommandShell::execute()
{
    IOH->mode = InputOutputMode::SHELL;
}

void CommandRotateCube::execute()
{
    double angle = 3.0 * M_PI / 180.0;
    dynamic_cast<ShapeBase3D*>(Screen::getInstance()->getObject("cube"))->rotateX(angle);
    dynamic_cast<ShapeBase3D*>(Screen::getInstance()->getObject("cube"))->rotateY(angle);
    dynamic_cast<ShapeBase3D*>(Screen::getInstance()->getObject("cube"))->rotateZ(angle);
}
