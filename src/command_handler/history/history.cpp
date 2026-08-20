#include "history.hpp"
#include "../../kernel/kernel.hpp"

void History::description()
{
    Kernel::IOH()->output("\tHistory of commands executed\n");
}

void History::man()
{
    Kernel::IOH()->output("\tThis command is used to display the history of commands executed.\n");
}

void History::execute()
{
    printHistory();
}

void History::printHistory()
{
    auto queue = commandQueue;

    while (! queue.empty())
    {
        const auto& entry = queue.front();

        Kernel::IOH()->output(
            FormatUtils::formatString(
                HISTFORMAT,
                entry.id,
                entry.name.c_str()));

        queue.pop();
    }
}

void History::addCommand(ICommand* command, std::string name)
{
    if (commandQueue.size() >= HISTSIZE)
    {
        commandQueue.pop();
    }

    commandQueue.push({nextId++, command, std::move(name)});
}