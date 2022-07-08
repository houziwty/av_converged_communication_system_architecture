#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "http_session.h"
#include "http_request_parser.h"
using namespace module::network::http;

HttpRequestParser::HttpRequestParser(
    HttpSession& http, 
    const uint32_t bytes/* = 3 * 1024 * 1024*/) 
    : httpSession{http}, buffer {nullptr}, bufBytes{ bytes }, pos{ 0 }
{
    buffer = new(std::nothrow) char[bufBytes];
}

HttpRequestParser::~HttpRequestParser()
{
    boost::checked_array_delete(buffer);
}

int HttpRequestParser::input(
    const void* request/* = nullptr*/, 
    const uint32_t bytes/* = 0 */)
{
    int ret{request && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        //缓存超界
        if (pos + bytes > bufBytes)
        {
            pos = 0;
            return Error_Code_Operate_Out_Of_Range;
        }

        //拷贝数据到缓存后再进行解析
        //解析一次完整包重置位置
        //否则就要移动数据内容到缓存首部
        ret = XMem().copy(request, bytes, buffer + pos, bytes);
        if (Error_Code_Success == ret)
        {
            pos += bytes;
            const int parsepos{parse(buffer, pos)};

            if (0 < parsepos && parsepos < pos)
            {
                const std::size_t len{pos - parsepos};
                ret = XMem().move(buffer + parsepos, buffer, len);
                pos = len;
            }
            else
            {
                pos = 0;
            }
        }
    }

    return ret;
}

int HttpRequestParser::parse(
    const char* data/* = nullptr*/,
    const uint32_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Operate_Failure};
    std::size_t curpos{0}, startpos{0};

    while (Error_Code_Success == ret && startpos < bytes)
    {
        const std::string http{ data + startpos, bytes - startpos };
        curpos = http.find_first_of("\r\n");

        if (!curpos)
        {
            //有消息体且长度足够就直接按大小拷贝
            if (headers.find("Content-Length") != headers.end())
            {
                const int len{atoi(headers["Content-Length"].c_str())};
                
                if (startpos + len <= bytes)
                {
                    //直接跳过消息头和消息体的分隔符\r\n
                    content = http.substr(2, len);
                    curpos += len;
                }
            }

            httpSession.afterParsedHttpRequestNotification(method, url, protocol, content, headers);
        }
        else if (0 < curpos)
        {
            const std::string line{http.substr(0, curpos)};
            const std::size_t splitpos{line.find_first_of(":")};

            if (std::string::npos == splitpos)
            {
                //第一行
                const std::size_t firstblackpos{line.find_first_of(" ")}, lastblackpos{line.find_last_of(" ")};
                method = line.substr(0, firstblackpos);
                url = line.substr(firstblackpos + 1, lastblackpos - firstblackpos - 1);
                protocol = line.substr(lastblackpos + 1, line.length());
            }
            else
            {
                //消息头单行
                const std::size_t colonpos{line.find_first_of(":")};
                headers.insert(std::make_pair(line.substr(0, colonpos), line.substr(colonpos + 1, line.length())));
            }
        }

        curpos += 2;
        startpos += curpos;
    }
    
    return Error_Code_Success == ret ? startpos : ret;
}
