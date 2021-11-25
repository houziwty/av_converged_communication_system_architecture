#include <iostream>
#include "message_dispatcher_service.h"

int main(int argc, const char** argv)
{
    std::cout << "It is beginning ..." << std::endl;

    MessageDispatcher md;
    md.start();

    md.stop();
    return 0;
}