#include "../main.hpp"
#include <iostream>
#include "history/history.hpp"
#include "command_handler.hpp"

CommandHandler* CommandHandler::m_instance = nullptr;

extern InputOutputHandler* IOH;

CommandHandler::CommandHandler()
    : history(std::make_unique<History>())
{
    initializeCommands();
}

void CommandHandler::initializeCommands()
{
    commandMap["help"] = std::make_unique<CommandHelp>();
    commandMap["screen"] = std::make_unique<CommandScreen>();
    commandMap["shell"] = std::make_unique<CommandShell>();
    commandMap["both"] = std::make_unique<CommandBoth>();
    commandMap["rotate"] = std::make_unique<CommandRotateCube>();
    commandMap["menu"] = std::make_unique<CommandMenu>();
    commandMap["history"] = std::unique_ptr<CommandBase>(history.get());
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

            History* history = dynamic_cast<History*>(commandMap["history"].get());
            if (history)
            {
                history->addCommand(it->second.get(), m_currentCommand);
            }
        }
        else
        {
            IOH->output("Unknow command. Use help");
        }
    }
}
