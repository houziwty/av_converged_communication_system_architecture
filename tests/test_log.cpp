#include <iostream>
#include "liblog/log.h"

int main(int argc, char* argv[])
{
    Logger l;
	l.init(argv[0]);
    return 0;
}