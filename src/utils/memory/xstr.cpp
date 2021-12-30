#ifdef _WINDOWS
#include <string.h>
#else
#include <cstring>
#endif//WINDOWS
#include <new>
#include "error_code.h"
#include "utils/memory/xstr.h"
using namespace framework::utils::memory;

XStr::XStr()
{}

XStr::~XStr()
{}

int XStr::copy(
    const char* src/* = nullptr*/, 
    const int bytes/* = 0*/, 
    char* dest/* = nullptr*/)
{
    int ret{src && dest && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
#ifdef _WINDOWS
	    strncpy_s(dest, bytes + 1, src, bytes);
#else
        strncpy(dest, src, bytes);
#endif//WINDOWS
    }

    return ret;
}

const char* XStr::copyNew(
    const char* data/* = nullptr*/, 
    const int bytes/* = 0*/)
{
    char* dest{nullptr};

    if (data && 0 < bytes)
    {
        dest = new(std::nothrow) char[bytes];
        
        if (dest)
        {
            copy(data, bytes, dest);
        }
    }
    
    return dest;
}