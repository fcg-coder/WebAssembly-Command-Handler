#include "commands.hpp"
#include "../../graphic_lib/screen.hpp"
#include "../../kernel/kernel.hpp"
#include "../menu/menu.hpp"

void CommandHelp::execute()
{
    const auto& commandMap = CommandRegister::getCommands();

    for (const auto& [commandName, command] : commandMap)
    {
        Kernel::IOH()->output(commandName + "\n");
        command->description();
    }
}

void CommandHelp::man()
{
}

void CommandHelp::description()
{
    Kernel::IOH()->output("\tThis command is used to display the list of available commands.");
}

void CommandScreen::execute()
{
    Kernel::IOH()->setMode(kernel::InputOutputMode::SCREEN);
}
void CommandScreen::man()
{
}

void CommandScreen::description()
{
    Kernel::IOH()->output("\tThis command is used to switch to screen mode.");
}
void CommandShell::execute()
{
    Kernel::IOH()->setMode(kernel::InputOutputMode::SHELL);
}
void CommandShell::man()
{
}
void CommandShell::description()
{
    Kernel::IOH()->output("\tThis command is used to switch to shell mode.");
}
void CommandRotateCube::execute()
{
    double angle = 3.0 * M_PI / 180.0;
    dynamic_cast<ShapeBase3D*>(Screen::getInstance().getObject("cube"))->rotateX(angle);
    dynamic_cast<ShapeBase3D*>(Screen::getInstance().getObject("cube"))->rotateY(angle);
    dynamic_cast<ShapeBase3D*>(Screen::getInstance().getObject("cube"))->rotateZ(angle);
}
void CommandRotateCube::man()
{
}
void CommandRotateCube::description()
{
    Kernel::IOH()->output("\tThis command is used to rotate the cube.");
}
void CommandMenu::execute()
{
    auto* menu = new Menu("You touched the computer today? Did it make you happy?", {"Yes", "No"});
}
void CommandMenu::man()
{
}
void CommandMenu::description()
{
    Kernel::IOH()->output("\tThis command is used to display a menu.");
}
