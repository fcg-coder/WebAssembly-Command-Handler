#pragma once

#include "interface.hpp"
#include "web_ioh.hpp"

class Kernel final
{
public:
    static void init(kernel::IInputOutputHandler& ioh)
    {
        instance = &ioh;
    }

    static kernel::IInputOutputHandler* IOH()
    {
        /**
         * @todo
         */
        if (instance == nullptr)
            Kernel::init(web::WebAsmIOH::getInstance());

        return instance;
    }

private:
    inline static kernel::IInputOutputHandler* instance = nullptr;
};