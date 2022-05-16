#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "memory/xbuf.h"
using namespace framework::utils::memory;

XBuffer::XBuffer(const uint64_t bytes/* = 3 * 1024 * 1024*/) 
    : capacity_bytes{0}, buf{nullptr}, buf_bytes{0}
{
    capacity(bytes);
}

XBuffer::XBuffer()
{
    reset();
}

int XBuffer::input(const char* data/* = nullptr*/, const uint64_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    //数据超界就重置
    if (buf_bytes + bytes > capacity_bytes)
    {
        reset();
        ret = capacity(bytes);
    }

    if (Error_Code_Success == ret)
    {   
#ifdef _WINDOWS
        memcpy_s(buf + buf_bytes, bytes, data, bytes);
#else
        memcpy(buf + buf_bytes, data, bytes);
#endif//_WINDOWS

        buf_bytes += bytes;
    }
    
    return ret;
}

int XBuffer::capacity(const uint64_t bytes/* = 0*/)
{
    //如果capacity_bytes为0，或者当前请求的_bytes > capacity_bytes时，需要重新分配内存
    if (!capacity_bytes || bytes > capacity_bytes)
    {
        boost::checked_array_delete(buf);
    }
    
    buf = new(std::nothrow) char[bytes + 1]{0};
    if (buf)
    {
        capacity_bytes = bytes;
    }
    
    return buf ? Error_Code_Success : Error_Code_Bad_New_Memory;
}

void XBuffer::reset()
{
    if (buf && 0 < buf_bytes)
    {
        boost::checked_array_delete(buf);
        buf = nullptr;
        buf_bytes = 0;
        capacity_bytes = 0;
    }
}

StringBuffer::StringBuffer() 
    : XBuffer(), head{0}, tail{0}
{}
StringBuffer::StringBuffer(std::string str) 
    : XBuffer(), stringbuf{std::move(str)}, head{0}, tail{0}
{}
StringBuffer::StringBuffer(const char* str/* = nullptr*/) 
    : XBuffer(), head{0}, tail{0}
{
    if (str)
    {
        stringbuf = std::move(str);
    }
}
StringBuffer::StringBuffer(StringBuffer&& sb) 
    : XBuffer(), stringbuf{std::move(sb.stringbuf)}, head{sb.head}, tail{sb.tail}
{}
StringBuffer::StringBuffer(const StringBuffer& sb) 
    : XBuffer(), stringbuf{std::move(sb.stringbuf)}, head{sb.head}, tail{sb.tail}
{}

StringBuffer::~StringBuffer()
{}

StringBuffer& StringBuffer::operator=(std::string str)
{
    stringbuf = std::move(str);
    head = 0;
    tail = 0;
    return *this;
}

StringBuffer& StringBuffer::operator=(const char* str/* = nullptr*/)
{
    if (str)
    {
        stringbuf = str;
    }
    head = 0;
    tail = 0;
    return *this;
}

StringBuffer& StringBuffer::operator=(StringBuffer&& sb) 
{
    stringbuf = sb.stringbuf;
    head = sb.head;
    tail = sb.tail;
    return *this;
}

StringBuffer& StringBuffer::operator=(const StringBuffer& sb)
{
    stringbuf = sb.stringbuf;
    head = sb.head;
    tail = sb.tail;
    return *this;
}

StringBuffer& StringBuffer::erase(const size_t pos/* = 0*/, const size_t n/* = std::string::npos*/)
{
    if (pos == 0)
    {
        //移除前面的数据
        if (n != std::string::npos)
        {
            //移除部分
            if (n < size())
            {
                head += n;
                data()[size()] = '\0';
            }
        }
        else
        {
            //移除全部数据
            head = 0;
            tail = stringbuf.size();
            data()[0] = '\0';
        }

        return *this;
    }

    if (n == std::string::npos || pos + n >= size()) 
    {
        //移除末尾所有数据
        if (pos < size())
        {
            tail += (size() - pos);
            data()[size()] = '\0';
        }
       
        return *this;
    }

    //移除中间的
    if (pos + n < size())
    {
        stringbuf.erase(head + pos, n);
    }
    
    return *this;
}

StringBuffer& StringBuffer::append(const StringBuffer& str)
{
    return append(str.data(), str.size());
}

StringBuffer& StringBuffer::append(const std::string& str)
{
    return append(str.data(), str.size());
}

StringBuffer& StringBuffer::append(const char* data/* = nullptr*/)
{
    return append(data, strlen(data));
}

StringBuffer& StringBuffer::append(const char* data/* = nullptr*/, const std::size_t len/* = 0*/)
{
    if (data && 0 < len)
    {
        if (head > stringbuf.capacity() / 2)
        {
            //清理[0, head]
            stringbuf.erase(0, head);
            head = 0;
        }

        //缓存有数据就从尾部追加，否则在头部追加
        if (!tail)
        {
            stringbuf.append(data, len);
        }
        else
        {
            stringbuf.insert(head + size(), data, len);
        }
    }

    return *this;
}

void StringBuffer::push_back(const char c)
{
    if (!tail) 
    {
        stringbuf.push_back(c);
    }
    else
    {
        data()[size()] = c;
        --tail;
        data()[size()] = '\0';
    }
}

StringBuffer& StringBuffer::insert(
    const std::size_t pos/* = 0*/, 
    const char* str/* = nullptr*/, 
    const std::size_t n/* = 0*/)
{
    if (pos != std::string::npos && str && 0 < n)
    {
        stringbuf.insert(head + pos, str, n);
    }

    return *this;
}

StringBuffer& StringBuffer::assign(const char* str/* = nullptr*/)
{
    return assign(str, strlen(str));
}

StringBuffer& StringBuffer::assign(const char* str/* = nullptr*/, const std::size_t len/* = 0*/)
{
    if (str && 0 < len)
    {
        if (str >= stringbuf.data() && str < stringbuf.data() + stringbuf.size())
        {
            head = str - stringbuf.data();
            if (str + len <= stringbuf.data() + stringbuf.size())
            {
                tail = stringbuf.data() + stringbuf.size() - (str + len);
            }
        }
        else
        {
            stringbuf.assign(str, len);
            head = 0;
            tail = 0;
        }
    }

    return *this;
}

void StringBuffer::reset()
{
    head = 0;
    tail = 0;
    stringbuf.clear();
}

char& StringBuffer::operator[](const std::size_t pos/* = std::string::npos*/);
{
    char c;
    
    if (pos < size())
    {
        c = data()[pos];
    }

    return c;
}

const char& StringBuffer::operator[](const std::size_t pos/* = std::string::npos*/) const;
{
    return (*const_cast<StringBuffer*>(this))[pos];
}

void StringBuffer::reserve(const std::size_t n/* = std::string::npos*/);
{
    if (std::string::npos != n)
    {
        stringbuf.reserve(n);
    }
}

void StringBuffer::resize(const std::size_t n/* = std::string::npos*/, const char c/* = 0*/);
{
    if (std::string::npos != n)
    {
        StringBuffer.resize(n, c);
        head = 0;
        tail = 0;
    }
}

const std::string StringBuffer::substr(
    const std::size_t pos/* = std::string::npos*/, 
    const std::size_t n/* = std::string::npos*/) const;
{
    std::string str;

    if (n == std::string::npos)
    {
        //获取末尾所有的
        if (pos < size())
        {
            str = stringbuf.substr(head + pos, size() - pos);
        }
    }
    else
    {
        //获取部分
        if (pos + n < size())
        {
            str = stringbuf.substr(head + pos, n);
        }
    }

    return str;
}
