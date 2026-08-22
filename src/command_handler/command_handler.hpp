#pragma once

#include <iostream>
#include "history/history.hpp"
#include "commands/commands.hpp"
#include "command_handler_base.hpp"
#include <memory>

namespace kernel
{

    class CommandHandler : public CommandHandlerBase<CommandHandler>
    {
        friend class CommandHandlerBase<CommandHandler>;

    private:
        void executeImpl(const std::string& command);
        CommandHandler()
            : history(std::make_unique<History>()),
              commandHelp(std::make_unique<CommandHelp>()),
              commandScreen(std::make_unique<CommandScreen>()),
              commandShell(std::make_unique<CommandShell>())
        {
        }

        std::string m_currentCommand;

        std::unique_ptr<History> history;
        std::unique_ptr<CommandHelp> commandHelp;
        std::unique_ptr<CommandScreen> commandScreen;
        std::unique_ptr<CommandShell> commandShell;
    };
} // namespace kernel