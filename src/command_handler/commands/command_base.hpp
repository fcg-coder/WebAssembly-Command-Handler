#pragma once

#include <unordered_map>
#include <string>
#include <utility>

class ICommand
{
public:
    explicit ICommand(std::string name)
        : name(std::move(name))
    {
    }

    virtual ~ICommand() = default;

    virtual void execute() = 0;
    virtual void man() = 0;
    virtual void description() = 0;

    const std::string& getName() const
    {
        return name;
    }

private:
    std::string name;
};

class CommandRegister
{
public:
    explicit CommandRegister(ICommand* command)
    {
        m_commandMap.emplace(command->getName(), command);
    }

    virtual ~CommandRegister() = default;

    static const std::unordered_map<std::string, ICommand*>& getCommands()
    {
        return m_commandMap;
    }

private:
    static inline std::unordered_map<std::string, ICommand*> m_commandMap;
};