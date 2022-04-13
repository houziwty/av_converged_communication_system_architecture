#include "boost/format.hpp"
#include "error_code.h"
#include "url/url.h"
using namespace framework::utils::data;
#include "server.h"
#include "consumer.h"

Consumer::Consumer(Server& svr) : Libxmq(), server{svr}
{}

Consumer::~Consumer()
{}

int Consumer::run(const uint32_t id/* = 0*/)
{
    int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        modeconf.id = id;
        modeconf.type = XMQModuleType::XMQ_MODULE_TYPE_WORKER;
        ret = addNode(modeconf);
    }

    return ret;
}

int Consumer::stop()
{
    int ret{0 < modeconf.id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        ret = removeNode(modeconf.id);
    }

    return ret;
}

void Consumer::afterPolledXMQDataNotification(
    const uint32_t id/* = nullptr*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
    const char* name/* = 0*/)
{
    //数据解析逻辑说明：
    // 1. 解析协议名称，如果是register则处理服务注册业务，否则到（2）；
    // 2. 解析协议名称和主机名称，与服务名称对比是否一致，如果是则由服务执行业务处理， 否则到（3）；
    // 3. 在注册服务表中查找主机名称，如果存在，则执行转发消息，否则应答消息不可达错误。
    
    Url url;
    int ret{ url.parse(data, bytes)};

    if(Error_Code_Success == ret)
    {
        if (!url.proto().compare("register"))
        {
            const std::vector<Parameter> items{url.parameters()};

            for(int i = 0; i != items.size(); ++i)
            {
                if(!items[i].key.compare("timestamp"))
                {
                    server.processRegisterRequest(name, std::atoll(items[i].value.c_str()));
                    const std::string rep{
                        (boost::format("register://%s?timestamp=%lld") % XMQHostID % items[i].value).str()};
                    send(modeconf.id, rep.c_str(), rep.length(), name);
                    break;
                }
            }
        }
        else if (!url.proto().compare("query") && 
                 !url.host().compare(XMQHostID))
        {
            const std::vector<Parameter> parameters{url.parameters()};
            const std::string names{server.processQueryRequest()};
            std::string rep{
                (boost::format("query://%s?%s") % XMQHostID % names).str()};

            for(int i = 0; i != parameters.size(); ++i)
            {
                const std::string param{
                    (boost::format("&%s=%s") % parameters[i].key % parameters[i].value).str()};
                rep.append(param);
            }

            send(modeconf.id, rep.c_str(), rep.length(), name);
        }
        else
        {
            const std::string to{ url.host()};

            if (!to.empty() && server.existed(to.c_str()))
            {
                forwardMessage(name, to.c_str(), data, bytes);
            }
        }
    }
    else
    {
        server.catchExceptionOfParsingUrl(data, bytes, name, ret);
    }
}

void Consumer::forwardMessage(
    const char* from/* = nullptr*/, 
    const char* to/* = nullptr*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)   
{
    //转发消息在XMQ服务端将源地址追加到数据尾部
    //数据目的端解析尾部from字段可获取源地址以回复消息
    const std::string raw{(const char*)data, bytes};
    const std::string msg{
        (boost::format("%s&from=%s") % raw % from).str()};
    send(modeconf.id, msg.c_str(), msg.length(), to);
}
