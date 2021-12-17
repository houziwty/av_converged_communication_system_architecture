#include "boost/algorithm/string.hpp"
#include "boost/format.hpp"
#include "error_code.h"
#include "utils/url/url.h"
using namespace framework::utils::url;

Url::Url()
{}

Url::~Url()
{}

int Url::parse(const std::string data)
{
    int ret{!data.empty() ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        const int colonPos{static_cast<const int>(data.find_first_of(':'))};
        const int questionPos{static_cast<const int>(data.find_first_of('?'))};
        const int totalLength{static_cast<const int>(data.length())};

        protocol = data.substr(0, colonPos);

        const std::string hostpath{ 
            data.substr(colonPos + 3, -1 < questionPos ? questionPos - colonPos - 3 : totalLength - colonPos - 2) };
        if (!hostpath.empty())
        {
            const int splashPos{static_cast<const int>(hostpath.find_first_of('/'))};
            host = hostpath.substr(0, splashPos);
            if (-1 < splashPos)
            {
                path = hostpath.substr(splashPos, hostpath.length());
            }
        }

        const std::string params{-1 < questionPos ? data.substr(questionPos + 1, totalLength - questionPos) : ""};
        if (!params.empty())
        {
            std::vector<std::string> items;
            boost::split(items, params, boost::is_any_of("&"));

            for(int i = 0; i !=items.size(); ++i)
            {
                std::vector<std::string> subitems;
                boost::split(subitems, items[i], boost::is_any_of("="));
                ParamItem subitem{subitems[0], subitems[1]};
                parameters.push_back(subitem);
            }
        }
    }
    
    return ret;
}

const std::string Url::encode()
{
    std::string url{
        (boost::format("%s://%s") % protocol % host).str()};
    const int paramNumber{static_cast<int>(parameters.size())};

    if (!path.empty())
    {
        url.append("/");
        url.append(path);
    }
    

    for (int i = 0; i != paramNumber; ++i)
    {
        if (0 == i)
        {
            url.append("?");
        }
        
        url.append(parameters[i].key);
        url.append("=");
        url.append(parameters[i].value);

        if (i < paramNumber - 1)
        {
            url.append("&");
        } 
    }

    return url;
}

int Url::setProtocol(const std::string name)
{
    protocol = name;
    return protocol.empty() ? Error_Code_Invalid_Param : Error_Code_Success;
}

int Url::setHost(const std::string host)
{
    this->host = host;
    return this->host.empty() ? Error_Code_Invalid_Param : Error_Code_Success;
}

int Url::setPath(const std::string path)
{
    this->path = path;
    return this->path.empty() ? Error_Code_Invalid_Param : Error_Code_Success;
}

int Url::addParameter(const std::string key, const std::string value)
{
    int ret{!key.empty() ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        //参数列表中可能存在key相同的项
        //必须支持这种参数列表
        if (Error_Code_Success == ret)
        {
            ParamItem item{key, value};
            parameters.push_back(item);
        }
    }

    return ret;
}

int Url::removeParameter(const std::string key)
{
    int ret{!key.empty() ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        for(std::vector<ParamItem>::iterator it = parameters.begin(); it != parameters.end(); ++it)
        {
            if (0 == key.compare(it->key))
            {
                parameters.erase(it);
                break;
            }
        }

        ret = Error_Code_Object_Not_Exist;
    }

    return ret;
}
