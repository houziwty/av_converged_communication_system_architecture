#ifdef _WINDOWS
#include <string.h>
#else
#include <cstring>
#endif//WINDOWS
#include <new>
#include "utils/memory/xstr.h"
using namespace framework::utils::memory;

XStr::XStr()
{}

XStr::~XStr()
{}

const void* XStr::copy(
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    char* dest{nullptr};

    //只需要判断源字节大小是否有效
    //外部使用可以传递空字节数据 
    if (/*data && */0 < bytes)
    {
        dest = new(std::nothrow) char[bytes + 1];
        
        if (dest)
        {
            dest[bytes] = 0;
#ifdef _WINDOWS
	        strncpy_s(dest, bytes + 1, (const char*)data, bytes);
#else
            strncpy(dest, (const char*)data, bytes);
#endif//WINDOWS
        }
    }
    
    return dest;
}