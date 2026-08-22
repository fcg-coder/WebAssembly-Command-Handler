#include "history.hpp"
#include "../../kernel/kernel.hpp"


static std::string formatString(const char* format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    return std::string(buffer);
}

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
        Kernel::IOH()->output(formatString(HISTFORMAT, entry.id,entry.name.c_str()));
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