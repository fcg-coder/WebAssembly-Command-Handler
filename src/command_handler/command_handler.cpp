#include <iostream>
#include "history/history.hpp"
#include "command_handler.hpp"
#include "../kernel/kernel.hpp"

 

void CommandHandler::execute(const std::string& command)
{
    const auto& commandMap = CommandRegister::getCommands();

    auto it = commandMap.find(command);

    if (it != commandMap.end())
    {
        it->second->execute();
        history->addCommand(it->second, command);
    }
    else
    {
        Kernel::IOH()->output("Unknown command. Available commands:\n");

        for (const auto& [name, cmd] : commandMap)
        {
            Kernel::IOH()->output(name + "\n");
        }
    }
}
