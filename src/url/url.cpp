#include "boost/algorithm/string.hpp"
#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;

Url::Url()
{}

Url::~Url()
{}

int Url::parse(const void* data/* = nullptr*/, const uint64_t bytes/* = 0*/)
{
    int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        try
        {
            const std::string str((const char*)data, bytes);
            const int colonPos{static_cast<const int>(str.find_first_of(':'))};
            const int questionPos{static_cast<const int>(str.find_first_of('?'))};
            const int totalLength{static_cast<const int>(str.length())};

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
            //参数项
            const std::string params{-1 < questionPos ? str.substr(questionPos + 1, totalLength - questionPos) : ""};
            if (!params.empty())
            {
                std::vector<std::string> param_items;
                boost::split(param_items, params, boost::is_any_of("&"));

                for(int i = 0; i != param_items.size(); ++i)
                {
                    std::vector<std::string> kv_item;
                    boost::split(kv_item, param_items[i], boost::is_any_of("="));

                    //数组越界访问无法异常捕获
                    if (2 == kv_item.size())
                    {
                        Parameter parameter{kv_item[0], kv_item[1]};
                        _params.push_back(parameter);
                    }
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
