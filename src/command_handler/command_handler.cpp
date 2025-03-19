#include "command_handler.hpp"
#include "../main.hpp"

CommandHandler *CommandHandler::m_instance = nullptr;

CommandHandler::CommandHandler()
{
    initializeCommands();
}

void CommandHandler::initializeCommands()
{
    commandMap["help"] = std::make_unique<CommandHelp>();
}

CommandHandler *CommandHandler::getInstance()
{
    if (m_instance == nullptr)
    {
        m_instance = new CommandHandler();
    }
    return m_instance;
}

void CommandHandler::setCommand(const std::string &command)
{
    m_current_command = command;
    handler();
}

std::string CommandHandler::getCommand() const
{
    return m_current_command;
}

void CommandHandler::handler()
{
    if (!m_current_command.empty())
    {
        auto it = commandMap.find(m_current_command);
        if (it != commandMap.end())
        {
            it->second->execute();
        }
        else
        {
            std::cout << "HELP handler";
        }
    }
}

void CommandHelp::execute()
{
    std::cout << "HELP handler";
}
