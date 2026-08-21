#pragma once

#include "terminal_ioh.hpp"
#include "../graphic_lib/screen.hpp"

namespace kernel
{
    enum class InputOutputMode
    {
        SHELL,
        SCREEN
    };

    template <typename ShellType, typename ScreenType>
    class KernelImpl
    {
    public:
        KernelImpl(ShellType& ioh, ScreenType& screen)
            : m_ioh(&ioh),
              m_screen(&screen)
        {
        }

        ShellType* IOH() { return m_ioh; }

        ScreenType* SCREEN() { return m_screen; }

        void setMode(InputOutputMode mode) { m_mode = mode; }

        InputOutputMode getMode() const { return m_mode; }

    private:
        ShellType* m_ioh;
        ScreenType* m_screen;

        InputOutputMode m_mode = InputOutputMode::SHELL;
    };

    using ShellType = TerminalIOH;
    using ScreenType = Screen;

    using Impl = KernelImpl<ShellType, ScreenType>;

} // namespace kernel

class Kernel final
{
public:
    static kernel::ShellType* IOH();

    static kernel::ScreenType* SCREEN();

    static void setMode(kernel::InputOutputMode mode);

    static kernel::InputOutputMode getMode();

private:
    static kernel::Impl m_impl;
};