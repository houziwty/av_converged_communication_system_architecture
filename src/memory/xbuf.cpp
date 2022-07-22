#include <new>
#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "memory/xmem.h"
#include "memory/xbuf.h"
using namespace framework::utils::memory;

XBuffer::XBuffer(const std::size_t capacity/* = 3 * 1024 * 1024*/) 
    : buffer{nullptr}, bufferCapacity{capacity}, currentLen{0}
{
    if (0 < bufferCapacity)
    {
        buffer = new(std::nothrow) char[bufferCapacity];
    }
}

XBuffer::~XBuffer()
{
    boost::checked_array_delete(buffer);
}

int XBuffer::input(const char* data/* = nullptr*/, const std::size_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        //缓存超界
        if (currentLen + bytes > bufferCapacity)
        {
            return Error_Code_Operate_Out_Of_Range;
        }

        //拷贝数据到缓存
        ret = XMem().copy(data, bytes, buffer + currentLen, bytes);
        if (Error_Code_Success == ret)
        {
            currentLen += bytes;
        }
    }
    
    return ret;
}

int XBuffer::move(const std::size_t pos/* = 0*/)
{
    int ret{0 < pos && 0 < currentLen && pos < currentLen ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        currentLen -= pos;
        ret = XMem().move(buffer + pos, buffer, currentLen); 
    }
    
    return ret;
}
