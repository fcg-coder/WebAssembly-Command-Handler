#include <algorithm>
#include <cctype>
#include <sstream>

#include "command_handler.hpp"
#include "../kernel/kernel.hpp"

namespace kernel
{
    void CommandHandler::executeImpl(const std::string& command)
    {
        // Убираем пробелы/табуляции/переводы строк с конца.
        std::string input = command;

        input.erase(std::find_if(input.rbegin(),input.rend(), [](unsigned char c) {  return !std::isspace(c); } ).base(),input.end());

        // Если после удаления пробелов строка пустая.
        if (input.empty())
            return;

        // Разбираем строку на слова.
        std::istringstream stream(input);

        std::string commandName;
        stream >> commandName;

        // Получаем аргументы.
        std::vector<std::string> args;

        std::string arg;

        while (stream >> arg)
        {
            args.push_back(arg);
        }

        const auto& commandMap = CommandRegister::getCommands();

        auto it = commandMap.find(commandName);

        if (it != commandMap.end())
        {
            it->second->execute(args);
            history->addCommand( it->second, input  );
            return;
        }

        Kernel::IOH()->output( "\nUnknown command %s. Use help", commandName.c_str());

    }

} // namespace kernel