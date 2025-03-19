#include "../main.hpp"
#include <iostream>

CommandHandler* CommandHandler::m_instance = nullptr;

inputOutputHandler* screen = inputOutputHandler::getInstance();

CommandHandler::CommandHandler()
{
    initializeCommands();
}

void CommandHandler::initializeCommands()
{
    commandMap["help"] = std::make_unique<CommandHelp>();
    commandMap["draw"] = std::make_unique<CommandDrawSquare>();
}

CommandHandler* CommandHandler::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new CommandHandler();
    }
    return m_instance;
}

void CommandHandler::setCommand(const std::string& command)
{
    m_currentCommand = command;
    handler();
}

std::string CommandHandler::getCommand() const
{
    return m_currentCommand;
}

void CommandHandler::handler()
{

    if (! m_currentCommand.empty())
    {
        auto it = commandMap.find(m_currentCommand);
        if (it != commandMap.end())
        {
            it->second->execute();
        }
        else
        {
            screen->output("Unknow command. Use help");
        }
    }
}
