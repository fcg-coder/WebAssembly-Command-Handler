#include "kernel.hpp"
#include "kernel_types.hpp"

kernel::Impl Kernel::m_impl(
    kernel::ShellType::getInstance(),
    kernel::ScreenType::getInstance(),
    kernel::CommandHandlerType::getInstance());

kernel::ShellType* Kernel::IOH()
{
    return m_impl.IOH();
}

kernel::ScreenType* Kernel::SCREEN()
{
    return m_impl.SCREEN();
}

void Kernel::executeCmd(const std::string& command)
{
    m_impl.executeCmd(command);
}

void Kernel::setMode(kernel::InputOutputMode mode)
{
    m_impl.setMode(mode);
}

kernel::InputOutputMode Kernel::getMode()
{
    return m_impl.getMode();
}