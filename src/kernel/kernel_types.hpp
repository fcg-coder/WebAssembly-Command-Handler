#pragma once

#include "../graphic_lib/screen.hpp"
#include "../command_handler/command_handler.hpp"
#include "terminal_ioh.hpp"
// #    include "web_ioh.hpp"

namespace kernel
{
    using ScreenType = Screen;
    using CommandHandlerType = CommandHandler;
    class TerminalShell;
    using ShellType = TerminalShell;
    //     using ShellType = WebAsmShell;

} // namespace kernel