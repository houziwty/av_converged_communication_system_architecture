//#include "boost/algorithm/string.hpp"
#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;

Url::Url()
{}

Url::~Url()
{}

int Url::parse(const char* data/* = nullptr*/)
{
    int ret{data ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        try
        {
            const std::string str(data);
            const std::size_t colonPos{str.find_first_of(':')};
            const std::size_t questionPos{str.find_first_of('?')};
            const std::size_t totalLength{str.length()};

            //协议
            _proto = str.substr(0, colonPos);
            //主机
            const std::string host_path{ 
                str.substr(colonPos + 3, -1 < questionPos ? questionPos - colonPos - 3 : totalLength - colonPos - 2) };
            if (!host_path.empty())
            {
                const int splashPos{static_cast<const int>(host_path.find_first_of('/'))};
                _host = host_path.substr(0, splashPos);
                //路径
                if (-1 < splashPos)
                {
                    _path = host_path.substr(splashPos, host_path.length());
                }
            }
            //针对特定的URL解析而非通用URL格式
            const std::string params{-1 < questionPos ? str.substr(questionPos + 1, totalLength - questionPos) : ""};
            if (!params.empty())
            {
                //from字段必须出现在data字段后面
                const std::size_t data_pos{params.find("data=")};
                const std::size_t from_pos{params.find("from=")};

                if (std::string::npos != from_pos)
                {
                    Parameter param_from{"from", params.substr(from_pos + 5, totalLength - from_pos - 5)};
                    _params.push_back(param_from);

                    Parameter param_data{"data", params.substr(data_pos + 5, from_pos - data_pos - 6)};
                    _params.push_back(param_data);
                }
                else
                {
                    Parameter param_data{"data", params.substr(data_pos + 5, totalLength - data_pos - 5)};
                    _params.push_back(param_data);
                }
            }
        }
        catch(...)
        {
            ret = Error_Code_Invalid_URL;
        }
    }
    
    return ret;
}
