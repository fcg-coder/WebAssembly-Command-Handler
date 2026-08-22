#include "commands.hpp"
#include "../../graphic_lib/screen.hpp"
#include "../../kernel/kernel.hpp"

void CommandHelp::execute()
{
    const auto& commandMap = CommandRegister::getCommands();

    for (const auto& [commandName, command] : commandMap)
    {
        Kernel::IOH()->output(commandName);
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
    Kernel::setMode(kernel::InputOutputMode::SCREEN);
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
    Kernel::setMode(kernel::InputOutputMode::SHELL);
}
void CommandShell::man()
{
}
void CommandShell::description()
{
    Kernel::IOH()->output("\tThis command is used to switch to shell mode.");
}


void CommandLoadSceneCube::execute()
{
    auto* screen = Kernel::SCREEN();

    if (!screen)
        return;

    screen->loadScene("cubeScene");
    Kernel::setMode(kernel::InputOutputMode::SCREEN);
}

void CommandLoadSceneCube::man()
{

}

void CommandLoadSceneCube::description()
{
    Kernel::IOH()->output("\tLoad and display the cube scene.");
}


void CommandLoadScenePyramid::execute()
{
    auto* screen = Kernel::SCREEN();

    if (!screen)
        return;
    screen->loadScene("pyramidScene");
    Kernel::setMode(kernel::InputOutputMode::SCREEN);

}

void CommandLoadScenePyramid::man()
{

}


void CommandLoadScenePyramid::description()
{
    Kernel::IOH()->output("\tLoad and display the pyramid scene.");
}