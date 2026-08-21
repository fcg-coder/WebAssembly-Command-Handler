#include "kernel/kernel.hpp"
#include "kernel/terminal_ioh.hpp"
#include "graphic_lib/screen.hpp"
#include <iostream>
#include <thread>
#include <chrono>

int main(int argc, char* argv[])
{
    Kernel::IOH()->output("Hello\n");
}