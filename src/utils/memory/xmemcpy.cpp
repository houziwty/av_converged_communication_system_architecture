#ifdef _WINDOWS
#include <string.h>
#else
#include <cstring>
#endif//WINDOWS
#include <new>
#include "error_code.h"
#include "utils/memory/xmemcpy.h"
using namespace framework::utils::memory;

XMemory::XMemory()
{}

XMemory::~XMemory()
{}

int XMemory::copy(
    const void* src/* = nullptr*/, 
    const int src_bytes/* = 0*/, 
    void* dest/* = nullptr*/, 
    const int dest_bytes/* = 0*/)
{
    int ret{src && 0 < src_bytes && dest && 0 < dest_bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        const int bytes{src_bytes > dest_bytes ? dest_bytes : src_bytes};

#ifdef _WINDOWS
	    memcpy_s(dest, bytes, src, bytes);
#else
        memcpy(dest, src, bytes);
#endif//WINDOWS
    }

    return ret;
}

void* XMemory::copyNew(
    const void* src/* = nullptr*/, 
    const int src_bytes/* = 0*/)
{
    void* dest{nullptr};

    if (src && 0 < src_bytes)
    {
        dest = new(std::nothrow) char[src_bytes];
        
        if (dest)
        {
            copy(src, src_bytes, dest, src_bytes);
        }
    }
    
    return dest;
}