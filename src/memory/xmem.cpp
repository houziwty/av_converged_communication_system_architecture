#ifdef _WINDOWS
#include <string.h>
#else
#include <cstring>
#endif//WINDOWS
#include <new>
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;

XMem::XMem()
{}

XMem::~XMem()
{}

int XMem::copy(
    const void* src/* = nullptr*/, 
    const uint64_t src_bytes/* = 0*/, 
    void* dest/* = nullptr*/, 
    const uint64_t dest_bytes/* = 0*/)
{
    int ret{src && 0 < src_bytes && dest && 0 < dest_bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        const uint64_t bytes{src_bytes > dest_bytes ? dest_bytes : src_bytes};

#ifdef _WINDOWS
	    memcpy_s(dest, bytes, src, bytes);
#else
        memcpy(dest, src, bytes);
#endif//WINDOWS
    }

    return ret;
}

void* XMem::alloc(
    const void* src/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    void* dest{nullptr};

    if (src && 0 < bytes)
    {
        dest = new(std::nothrow) uint8_t[bytes];
        
        if (dest)
        {
            copy(src, bytes, dest, bytes);
        }
    }
    
    return dest;
}

int XMem::move(
    const void* src/* = nullptr*/, 
    void* dest/* = nullptr*/, 
    const uint64_t dest_bytes/* = 0*/)
{
    int ret{src && dest && 0 < dest_bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        memmove(dest, src, dest_bytes);
    }
    
    return ret;
}

int XMem::zero(
    void* src/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    int ret{src && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        memset(src, 0, bytes);
    }
    
    return ret;
}
