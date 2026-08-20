#include "kernel/kernel.hpp"
#include "kernel/web_ioh.hpp"

int main()
{
    Kernel::init(web::WebAsmIOH::getInstance());

    Kernel::IOH()->output("Hello\n");

    return 0;
}