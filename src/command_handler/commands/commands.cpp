#include "commands.hpp"
#include "../../graphic_lib/screen.hpp"
#include "../menu/menu.hpp"

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

void CommandMenu::execute()
{
    auto* menu = new Menu("You touched the computer today? Did it make you happy?", {"Yes", "No"});
}
