#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "command_base.hpp"

class CommandHelp : public ICommand, public CommandRegister
{
public:
    CommandHelp()
        : ICommand("help"), CommandRegister(this)
    {
    }

    void execute() override;
    void man() override;
    void description() override;
};

class CommandScreen : public ICommand, public CommandRegister
{
public:
    CommandScreen()
        : ICommand("screen"), CommandRegister(this)
    {
    }

    void execute() override;
    void man() override;
    void description() override;
};

class CommandShell : public ICommand, public CommandRegister
{
public:
    CommandShell()
        : ICommand("shell"), CommandRegister(this)
    {
    }

    void execute() override;
    void man() override;
    void description() override;
};
