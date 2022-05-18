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
                ret = parseOneLine(line.c_str());
                //解析异常就退出
                if (Error_Code_Success != ret)
                {
                    break;
                }
                
                str = str.substr(pos, str.length() - pos);
            }
            else
            {
                break;
            }
        }
    }
    
    return ret;
}

const std::vector<std::string> UrlParser::value(const std::string& key)
{
    std::vector<std::string> values;
    auto range{_headers.equal_range(key)};

    for (auto i = range.first; i != range.second; ++i)
    {
        values.push_back(i->second);
    }
    
    return std::move(values);
}

int UrlParser::parseOneLine(const char* line/* = nullptr*/)
{
    int ret{line ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        const std::string str{line};

        if (std::string::npos != str.find("HTTP"))
        {
            //解析HTTP命令、URL和类型
            const std::size_t first{str.find_first_of(" ")}, last{str.find_last_of(" ")};
            _method = str.substr(0, first);
            _url = str.substr(first + 1, last - first - 1);
            _proto = str.substr(last + 1, str.length() - last);
        }
        else if (std::string::npos != str.find(": "))
        {
            //解析消息头参数
            const std::size_t pos{str.find_first_of(": ")};
            const std::string key{str.substr(0, pos)};
            const std::string value{str.substr(pos + 2, str.length() - pos - 2)};
            _headers.insert(std::make_pair(key, value));
        }
        else
        {
            ret = Error_Code_Bad_RequestUrl;
        }
    }
    
    return ret;
}
