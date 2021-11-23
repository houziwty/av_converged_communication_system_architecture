#include "utils/memory/xmemcpy.h"
using framework::utils::memory;

void xmemcpy(
    void* src/* = nullptr*/, 
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