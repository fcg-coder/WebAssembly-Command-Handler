#include "kernel.hpp"

kernel::Impl Kernel::m_impl {kernel::ShellType::getInstance(), kernel::ScreenType::getInstance()};

kernel::ShellType* Kernel::IOH()
{
    return m_impl.IOH();
}

kernel::ScreenType* Kernel::SCREEN()
{
    return m_impl.SCREEN();
}

void Kernel::setMode(kernel::InputOutputMode mode)
{
    m_impl.setMode(mode);
}

kernel::InputOutputMode Kernel::getMode()
{
    return m_impl.getMode();
}