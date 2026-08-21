
#include <iostream>
#include <thread>
#include <chrono>

#include "kernel/kernel.hpp"

int main()
{
    Kernel::IOH()->output("Terminal started!\n");

    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    return 0;
}