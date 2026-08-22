#pragma once

#include <string>
#include <type_traits>
#include "kernel_types.hpp"

namespace kernel
{
    enum class InputOutputMode
    {
        SHELL,
        SCREEN
    };

    /**
     * @b PIMPL + template
     */
    template <typename ShellType, typename ScreenType, typename CommandHandlerType>
    class KernelImpl final
    {
        static_assert(std::is_class_v<ShellType>, "ShellType must be a class");
        static_assert(std::is_class_v<ScreenType>, "ScreenType must be a class");
        static_assert(std::is_class_v<CommandHandlerType>, "CommandHandlerType must be a class");

        // проверка что не const
        static_assert(!std::is_const_v<ShellType>);
        static_assert(!std::is_const_v<ScreenType>);
        static_assert(!std::is_const_v<CommandHandlerType>);

        // все классы в текущей реализации наследоуются от шаблона с getInstance который возвращает ссылку
        static_assert(!std::is_reference_v<ShellType>);
        static_assert(!std::is_reference_v<ScreenType>);
        static_assert(!std::is_reference_v<CommandHandlerType>);

    public:
        KernelImpl(ShellType& ioh, ScreenType& screen, CommandHandlerType& commandHandler)
            : m_ioh(&ioh), m_screen(&screen), m_commandHandler(&commandHandler)
        {
        }

        ShellType* IOH() { return m_ioh; }
        ScreenType* SCREEN() { return m_screen; }
        void executeCmd(const std::string& command) { m_commandHandler->execute(command); }
        void setMode(InputOutputMode mode) { m_mode = mode; }
        InputOutputMode getMode() const { return m_mode; }

    private:
        ShellType* m_ioh;
        ScreenType* m_screen;
        CommandHandlerType* m_commandHandler;
        InputOutputMode m_mode = InputOutputMode::SHELL;
    };

    using Impl = KernelImpl<ShellType, ScreenType, CommandHandlerType>;

} // namespace kernel

class Kernel final
{
public:
    static kernel::ShellType* IOH();
    static kernel::ScreenType* SCREEN();
    static void executeCmd(const std::string& command);
    static void setMode(kernel::InputOutputMode mode);
    static kernel::InputOutputMode getMode();

private:
    static kernel::Impl m_impl;
};