#ifndef WINDOWS
#include <cstring>
#endif//WINDOWS
#include "utils/memory/xstr.h"
using namespace framework::utils::memory;

XStr::XStr()
{}

XStr::~XStr()
{}

void XStr::copy(
    const char* src/* = nullptr*/, 
    char* dest/* = nullptr*/, 
    const unsigned long long bytes/* = 0*/)
{
    if(src && dest && 0 < bytes)
    {
#ifdef WINDOWS
	    strcpy_s(dest, bytes, src, bytes);
#else
        strcpy(dest, src);
#endif//WINDOWS
    }
}