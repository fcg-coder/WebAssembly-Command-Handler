#pragma once

#include "../graphic_lib/screen.hpp"
#include "../command_handler/command_handler.hpp"

#ifdef USE_WEBASM
#    include "web_ioh.hpp"
#elif defined(USE_TERMINAL)
#    include "terminal_ioh.hpp"
#endif

namespace kernel
{
    using CommandHandlerType = CommandHandler;
    using ScreenType = Screen;

#ifdef USE_WEBASM

    class WebAsmShell;
    using ShellType = WebAsmShell;

#elif defined(USE_TERMINAL)

    class TerminalShell;
    using ShellType = TerminalShell;

#else

#    error "Either USE_WEBASM or USE_TERMINAL must be defined"

#endif

} // namespace kernel