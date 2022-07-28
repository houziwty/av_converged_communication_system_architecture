#include "error_code.h"
#include "http_request_parser.h"
using namespace module::network::http;

HttpRequestParser::HttpRequestParser(
    AfterParsedHttpRequestCallback callback) 
    : afterParsedHttpRequestCallback{callback}
{}

HttpRequestParser::~HttpRequestParser()
{}

const std::size_t HttpRequestParser::parse(
    const void* data/* = nullptr*/,
    const std::size_t bytes/* = 0*/)
{
    std::size_t pos{0}, crlfPos{0};
    std::string method, url, protocol, type, content;
    std::multimap<std::string, std::string> headers;

    while (data && 0 < bytes && pos < bytes)
    {
        const std::string req{ (char*)data + pos, bytes - pos };
        crlfPos = req.find_first_of("\r\n");

        //如果0 == currentLinePos表示找到HTTP协议的头和消息体的分隔符
        if (!crlfPos)
        {
            //有消息体且长度足够就直接处理
            //否则缓存到下次接收后再解析
            std::multimap<std::string, std::string>::iterator content_len{headers.find("Content-Length")};
            std::multimap<std::string, std::string>::iterator content_type{headers.find("Content-Type")};
            if (content_len != headers.end() && content_type != headers.end())
            {
                const int len{atoi(content_len->second.c_str())};
                
                if (pos + len + 2 <= bytes)
                {
                    //直接跳过消息头和消息体的分隔符\r\n
                    content = req.substr(2, len);
                    type = content_type->second;
                    pos += len;
                }
                else
                {
                    //数据接收不完整
                    //等待下一次接收完成后再解析
                    pos = 0;
                    break;
                }
            }
        }
        else if (0 < crlfPos)
        {
            const std::string currentLine{req.substr(0, crlfPos)};
            const std::size_t colonPos{currentLine.find_first_of(":")};

            //通过查找单行中是否包含":"来区分当前行是否是请求或应答的首行还是字段
            if (std::string::npos == colonPos)
            {
                const std::size_t firstblackpos{currentLine.find_first_of(" ")}, lastblackpos{currentLine.find_last_of(" ")};
                method = currentLine.substr(0, firstblackpos);
                url = currentLine.substr(firstblackpos + 1, lastblackpos - firstblackpos - 1);
                protocol = currentLine.substr(lastblackpos + 1, currentLine.length());
            }
            else
            {
                //消息头单行
                const std::size_t colonpos{currentLine.find_first_of(":")};
                headers.insert(std::make_pair(currentLine.substr(0, colonpos), currentLine.substr(colonpos + 1, currentLine.length())));
            }
        }
        else
        {
            //TODO: 怎么解决多行表示一个字段的解析
            pos = 0;
            break;
        }

        pos += (crlfPos + 2);
    }

    //解析完整的请求才回调数据
    if (0 < pos && pos <= bytes && afterParsedHttpRequestCallback)
    {
        afterParsedHttpRequestCallback(method, url, protocol, headers, content);
    }
    
    return pos;
}
