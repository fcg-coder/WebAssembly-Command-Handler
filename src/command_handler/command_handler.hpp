#pragma once

#include <string>
#include <map>
#include <memory>
#include "../main.hpp"

class CommandBase
{
public:
    virtual ~CommandBase() = default;
    virtual void execute() = 0;
    

};

class CommandHelp : public CommandBase
{
public:
    void execute() override;
};

class CommandHandler
{
private:
    std::string m_current_command;
    static CommandHandler *m_instance;
    std::map<std::string, std::unique_ptr<CommandBase>> commandMap;

    CommandHandler();
    void initializeCommands();
    
public:
    static CommandHandler *getInstance();
    void setCommand(const std::string &command);
    std::string getCommand() const;
    void handler();
    
};

