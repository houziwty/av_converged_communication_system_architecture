#include <cstring>
#include "error_code.h"
#include "http_request_splitter.h"
using namespace module::network::http;

const std::size_t HttpRequestSplitter::kMaxCacheSize = 1 * 1024 * 1024;

HttpRequestSplitter::HttpRequestSplitter() 
    : buffer(nullptr), buffer_bytes{0}, content_bytes{0}
{}

HttpRequestSplitter::~HttpRequestSplitter()
{}

int HttpRequestSplitter::input(
    const char* data/* = nullptr*/, 
    std::size_t len/* = std::string::npos*/)
{
    int ret{data && 0 < len ? Error_Code_Success : Error_Code_Invalid_Param};

    if(Error_Code_Success == ret)
    {
        if (buffer_bytes > kMaxCacheSize)
        {
            //缓存超界
            reset();
            return Error_Code_Operate_Out_Of_Range;
        }

        const char* ptr{data};
        if(!buffer.empty())
        {
            buffer.append(data,len);
            data = ptr = buffer.data();
            len = buffer.size();
        }

    splitPacket:
        /*确保ptr最后一个字节是0，防止strstr越界
        *由于ZLToolKit确保内存最后一个字节是保留未使用字节并置0，
        *所以此处可以不用再次置0
        *但是上层数据可能来自其他渠道，保险起见还是置0
        */

        char &tail_ref = ((char*)ptr)[len];
        char tail_tmp = tail_ref;
        tail_ref = 0;

        //数据按照请求头处理
        const char* index{nullptr};
        buffer_bytes = len;
        while (0 == content_bytes && buffer_bytes > 0 && 
                (index = findLineTail(ptr, buffer_bytes)) != nullptr)
        {
            if (index == ptr)
            {
                break;
            }

            if (index < ptr || index > ptr + buffer_bytes)
            {
                return Error_Code_Operate_Out_Of_Range;
            }

            //_content_len == 0，这是请求头
            const char* header_ptr{ptr};
            std::size_t header_size = index - ptr;
            ptr = index;
            buffer_bytes = (len - (ptr - data));
            afterRecvHttpHeaderNotification(header_ptr, content_bytes);
        }

        if(0 >= buffer_bytes)
        {
            //没有剩余数据，清空缓存
            buffer.reset();
            return Error_Code_Success;
        }

        /*
        * 恢复末尾字节
        * 移动到这来，目的是防止HttpRequestSplitter::reset()导致内存失效
        */
        tail_ref = tail_tmp;

        if(!content_bytes)
        {
            //尚未找到http头，缓存定位到剩余数据部分
            buffer.assign(ptr, buffer_bytes);
            return Error_Code_Operate_Failure;
        }
        else
        {
            //数据按照固定长度content处理
            if(buffer_bytes < content_bytes)
            {
                //数据不够，缓存定位到剩余数据部分
                buffer.assign(ptr, buffer_bytes);
                return Error_Code_Operate_Failure;
            }

            //收到content数据，并且接受content完毕
            afterRecvHttpContentNotification(ptr, content_bytes);

            buffer_bytes -= content_bytes;
            ptr += content_bytes;
            //content处理完毕,后面数据当做请求头处理
            content_bytes = 0;

            if(0 < buffer_bytes)
            {
                //还有数据没有处理完毕
                buffer.assign(ptr, buffer_bytes);
                data = ptr = (char*)buffer.data();
                len = buffer.size();
                goto splitPacket;
            }

            buffer.reset();
            return Error_Code_Success;
        }

        //_content_len < 0;数据按照不固定长度content处理
        afterRecvHttpContentNotification(ptr, buffer_bytes);//消费掉所有剩余数据
        buffer.reset();
    }

    return ret;
}

void HttpRequestSplitter::afterRecvHttpContentNotification(
    const char* data/* = nullptr*/, 
    const std::size_t bytes/* = 0*/)
{}

void HttpRequestSplitter::reset()
{
    content_bytes = 0;
    buffer_bytes = 0;
    buffer.reset();
}

const char* HttpRequestSplitter::findLineTail(
    const char* data/* = nullptr*/, const std::size_t bytes/* = 0*/)
{
    const char* pos{strstr(data,"\r\n\r\n")};
    return pos ? pos + 4 : nullptr;
}
