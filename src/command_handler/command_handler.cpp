#include <iostream>
#include "history/history.hpp"
#include "command_handler.hpp"
#include "../kernel/kernel.hpp"

namespace kernel
{
    void CommandHandler::executeImpl(const std::string& command)
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
            /**
             * @todo \n
             */
            Kernel::IOH()->output("\nUnknown command %s. Available commands:\n", command.c_str());

            for (const auto& [name, cmd] : commandMap)
            {
                Kernel::IOH()->output(name + "\n");
            }
        }
    }

} // namespace kernel