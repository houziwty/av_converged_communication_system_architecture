#include <iostream>
#include "MessageDistribute.h"

int main(int argc, const char** argv)
{
    std::cout << "It is beginning ..." << std::endl;

    MessageDistribute md;
    md.start();

    md.stop();
    return 0;
}