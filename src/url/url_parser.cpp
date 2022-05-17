#include "error_code.h"
#include "url/url_parser.h"
using namespace framework::utils::url;

UrlParser::UrlParser()
{}

UrlParser::~UrlParser()
{}

int UrlParser::parse(const char* data/* = nullptr*/)
{
    int ret{data ? Error_Code_Success : Error_Code_Invalid_Param};
    int parsed_bytes{0};

    if (Error_Code_Success == ret)
    {
        //1.逐行提取消息数据
        //2.解析每一行的数据
        const std::string splash{"\r\n"};
        std::string str{data};
        std::size_t pos{0};

        while (1)
        {
            pos = str.find_first_of(splash);
            if (0 < pos)
            {
                const std::string line{str.substr(0, pos)};
                pos += splash.length();
                parsed_bytes += pos;
                parseOneLine(line.c_str());
                str = str.substr(pos, str.length() - pos);
            }
            else
            {
                break;
            }
        }
    }
    
    return 0 < parsed_bytes ? parsed_bytes : ret;
}

int UrlParser::parseOneLine(const char* line/* = nullptr*/)
{
    int ret{line ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        const std::string str{line};

        if (0 < str.find("HTTP"))
        {
            //解析HTTP命令、URL和类型
            const std::size_t first{str.find_first_of(" ")}, last{str.find_last_of(" ")};
            method = str.substr(0, first);
            api = str.substr(first + 1, last - first - 1);
            proto = str.substr(last + 1, str.length() - last);
        }
        else
        {
            //解析消息头参数
        }
    }
    
    return ret;
}
