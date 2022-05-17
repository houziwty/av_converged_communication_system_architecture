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
        //逐行提取消息头
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
        /* code */
    }
    
    return ret;
}
