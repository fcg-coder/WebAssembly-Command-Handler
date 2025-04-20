#include "commands.hpp"
#include "../../graphic_lib/screen.hpp"
#include "../menu/menu.hpp"

extern InputOutputHandler* IOH;

void CommandHelp::execute()
{
    const auto& commandMap = CommandHandler::getInstance()->getCommandMap();

    for (const auto& pair : commandMap)
    {

        CommandBase* cmd = pair.second.get();
        std::string commandName = pair.first;
        IOH->output(commandName);
        cmd->description();
    }
}

void CommandHelp::man()
{
}

void CommandHelp::description()
{
    IOH->output("\tThis command is used to display the list of available commands.");
}

void CommandScreen::execute()
{
    IOH->mode = InputOutputMode::SCREEN;
}
void CommandScreen::man()
{
}

void CommandScreen::description()
{
    IOH->output("\tThis command is used to switch to screen mode.");
}
void CommandShell::execute()
{
    IOH->mode = InputOutputMode::SHELL;
}
void CommandShell::man()
{
}
void CommandShell::description()
{
    IOH->output("\tThis command is used to switch to shell mode.");
}
void CommandRotateCube::execute()
{
    double angle = 3.0 * M_PI / 180.0;
    dynamic_cast<ShapeBase3D*>(Screen::getInstance()->getObject("cube"))->rotateX(angle);
    dynamic_cast<ShapeBase3D*>(Screen::getInstance()->getObject("cube"))->rotateY(angle);
    dynamic_cast<ShapeBase3D*>(Screen::getInstance()->getObject("cube"))->rotateZ(angle);
}
void CommandRotateCube::man()
{
}
void CommandRotateCube::description()
{
    IOH->output("\tThis command is used to rotate the cube.");
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
    IOH->output("\tThis command is used to display a menu.");
}