#include "history.hpp"

Queue* Queue::m_instance = nullptr;
extern InputOutputHandler* IOH;

History::History()
    : commandQueue(Queue::getInstance())
{
}
void History::description()
{
    IOH->output("\tHistory of commands executed.");
}

void History::man()
{
    IOH->output("\tThis command is used to display the history of commands executed.\n");
}

void History::execute()
{
    printHistory();
}

void History::printHistory()
{
    commandQueue->forEach([](int id, CommandBase* cmd, const std::string& name) {
        IOH->output(FormatUtils::formatString(HISTFORMAT, id, name.c_str()));
    });
}

void History::addCommand(CommandBase* command, std::string name)
{

    commandQueue->push(command, name);
    if (commandQueue->getSize() > HISTSIZE)
    {
        CommandBase* oldCmd = commandQueue->pop();
    }
}