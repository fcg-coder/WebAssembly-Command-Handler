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
};

class CommandHelp : public CommandBase
{
public:
    void execute() override;
};

class CommandScreen : public CommandBase
{
public:
    void execute() override;
};

class CommandShell : public CommandBase
{
public:
    void execute() override;
};

class CommandRotateCube : public CommandBase
{
public:
    void execute() override;
};