#pragma once

namespace kernel
{
    /**
     * @b CRTP
     */
    template <typename Derived>
    class CommandHandlerBase
    {
    public:
        static Derived& getInstance()
        {
            static Derived instance;
            return instance;
        }

        void execute(const std::string& command)
        {
            derived->executeImpl(command);
        }

    protected:
        CommandHandlerBase()
            : derived(static_cast<Derived*>(this))
        {
        }

    private:
        Derived* derived;
    };

} // namespace kernel