#pragma once

#include <string>
#include <memory>
#include "commands/commands.hpp"
#include "history/history.hpp"

class CommandHandler
{
public:
    static CommandHandler& getInstance()
    {
        static CommandHandler instance;
        return instance;
    }

    void execute(const std::string& command);

private:
    CommandHandler()
        : history(std::make_unique<History>()),
          commandHelp(std::make_unique<CommandHelp>()),
          commandScreen(std::make_unique<CommandScreen>()),
          commandShell(std::make_unique<CommandShell>()),
          commandRotateCube(std::make_unique<CommandRotateCube>()),
          commandMenu(std::make_unique<CommandMenu>())
    {
    }

    std::string m_currentCommand;

    std::unique_ptr<History> history;
    std::unique_ptr<CommandHelp> commandHelp;
    std::unique_ptr<CommandScreen> commandScreen;
    std::unique_ptr<CommandShell> commandShell;
    std::unique_ptr<CommandRotateCube> commandRotateCube;
    std::unique_ptr<CommandMenu> commandMenu;
};