#include "commands.hpp"
#include "../../graphic_lib/screen.hpp"
#include "../../kernel/kernel.hpp"

void CommandHelp::execute(const std::vector<std::string>& args)
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

void CommandScreen::execute(const std::vector<std::string>& args)
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
void CommandShell::execute(const std::vector<std::string>& args)
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


void CommandLoadScene::execute(const std::vector<std::string>& args)
{
    auto* screen = Kernel::SCREEN();

    if (!screen)
        return;

    if (args.size() != 1)
    {
        Kernel::IOH()->output("Usage: load_scene <scene_name>\n");
        return;
    }

    if (args[0].empty())
    {
        Kernel::IOH()->output("Scene name cannot be empty.\n");
        return;
    }

    if (screen->loadScene(args[0])) {
        Kernel::setMode(kernel::InputOutputMode::SCREEN);
    }
    else {
        Kernel::IOH()->output("Unknown scene\n");
    }
}

void CommandLoadScene::man()
{

}

void CommandLoadScene::description()
{
    Kernel::IOH()->output("\tLoad and display the cube scene.");
}

