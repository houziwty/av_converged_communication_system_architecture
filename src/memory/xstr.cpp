#ifdef _WINDOWS
#include <string.h>
#else
#include <cstring>
#endif//WINDOWS
#include <new>
#include "error_code.h"
#include "memory/xstr.h"
using namespace framework::utils::memory;

XStr::XStr()
{}

XStr::~XStr()
{}

int XStr::copy(
    const char* src/* = nullptr*/, 
    const uint64_t src_bytes/* = 0*/, 
    char* dest/* = nullptr*/, 
    const uint64_t dest_bytes/* = 0*/)
{
    int ret{src && 0 < src_bytes && dest && 0 < dest_bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        const uint64_t bytes{src_bytes > dest_bytes ? dest_bytes : src_bytes};

#ifdef _WINDOWS
	    strncpy_s(dest, bytes, src, bytes - 1);
#else
        strncpy(dest, src, bytes - 1);
#endif//WINDOWS
    }

    return ret;
}

char* XStr::alloc(
    const char* src/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    char* dest{nullptr};

    if (src && 0 < bytes)
    {
        dest = new(std::nothrow) char[bytes];
        
        if (dest)
        {
            copy(src, bytes, dest, bytes);
        }
    }
    
    return dest;
}
