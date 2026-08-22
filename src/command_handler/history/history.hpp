#pragma once

#include <cstdarg>
#include <cstdio>
#include <queue>
#include <string>
#include <utility>

#include "../commands/command_base.hpp"


#define HISTSIZE   100
#define HISTFORMAT "%d %s"

class History : public ICommand, public CommandRegister
{

public:
    History()
        : ICommand("history"), CommandRegister(this) {
          };
    void addCommand(ICommand* command, std::string name);
    void printHistory();

    void execute() override;
    void man() override;
    void description() override;

private:
    struct HistoryEntry
    {
        int id;
        ICommand* command;
        std::string name;
    };

    std::queue<HistoryEntry> commandQueue;
    int nextId = 0;
};