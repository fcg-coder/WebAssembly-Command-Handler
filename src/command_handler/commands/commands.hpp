#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <sstream>
#include "../../main.hpp"

class CommandBase
{
public:
    virtual ~CommandBase() = default;
    virtual void execute() = 0;
    virtual void man() = 0;
    virtual void description() = 0;
};

class CommandHelp : public CommandBase
{
public:
    void execute() override;
    void man() override;
    void description() override;
};

class CommandScreen : public CommandBase
{
public:
    void execute() override;
    void man() override;
    void description() override;
};

class CommandShell : public CommandBase
{
public:
    void execute() override;
    void man() override;
    void description() override;
};

class CommandBoth : public CommandBase
{
public:
    void execute() override;
    void man() override;
    void description() override;
};

class CommandRotateCube : public CommandBase
{
public:
    void execute() override;
    void man() override;
    void description() override;
};

class CommandMenu : public CommandBase
{
public:
    void execute() override;
    void man() override;
    void description() override;
};
