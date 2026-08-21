#pragma once

#include <cstdint>

#include "shapes/shape_base.hpp"

namespace kernel
{
    /**
     * @b CRTP
     * @b singleton как обязательное апи класса
     */
    template <typename Derived>
    class ScreenBase
    {
    public:
        static Derived& getInstance()
        {
            static Derived instance;
            return instance;
        }

        uint32_t* getScreen()
        {
            return derived().getScreen();
        }

        void addShape(Pixel p, int layoutIndex)
        {
            derived().addShape(p, layoutIndex);
        }

        void setSize(uint h, uint w)
        {
            derived().setSize(h, w);
        }

        void addObject(const char* key, ShapeBase* shape)
        {
            derived().addObject(key, shape);
        }

        void removeObject(const char* key)
        {
            derived().removeObject(key);
        }

        auto getSize()
        {
            return derived().getSize();
        }

    protected:
        ScreenBase() = default;
        ~ScreenBase() = default;

    private:
        Derived& derived()
        {
            return static_cast<Derived&>(*this);
        }

        const Derived& derived() const
        {
            return static_cast<const Derived&>(*this);
        }
    };

} // namespace kernel