#ifndef WINDOWS
#include <cstring>
#endif//WINDOWS
#include "utils/memory/xmemcpy.h"
using namespace framework::utils::memory;

XMemory::XMemory()
{}

XMemory::~XMemory()
{}

void XMemory::copy(
    const void* src/* = nullptr*/, 
    void* dest/* = nullptr*/, 
    const unsigned long long bytes/* = 0*/)
{
    if(src && dest && 0 < bytes)
    {
#ifdef WINDOWS
	    memcpy_s(dest, bytes, src, bytes);
#else
        memcpy(dest, src, bytes);
#endif//WINDOWS
    }
}