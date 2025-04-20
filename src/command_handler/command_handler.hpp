#pragma once

#include <string>
#include <map>
#include <memory>
#include "../main.hpp"
#include "commands/commands.hpp"

class CommandBase;

class CommandHandler
{
private:
    std::string m_currentCommand;
    static CommandHandler* m_instance;
    std::map<std::string, std::unique_ptr<CommandBase>> commandMap;

    CommandHandler();
    void initializeCommands();

public:
    const std::map<std::string, std::unique_ptr<CommandBase>>& getCommandMap() const
    {
        return commandMap;
    }
    static CommandHandler* getInstance();
    void setCommand(const std::string& command);
    std::string getCommand() const;
    void handler();
};
