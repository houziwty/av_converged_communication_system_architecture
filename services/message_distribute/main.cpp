#include <iostream>
#include "libxmq/libxmq.h"
using namespace module::libxmq;

int main(int argc, const char** argv)
{
    std::cout << "It is beginning ..." << std::endl;

    Router r;
    r.bind("127.0.0.1", 10000);
    r.send("xyz", "123");
}