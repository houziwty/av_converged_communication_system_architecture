#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "xmq_host_server.h"

XmqHostServer::XmqHostServer(FileLog& log) 
    : XMQNode(), fileLog{log}, stopped{false}, expire{nullptr}
{}

XmqHostServer::~XmqHostServer()
{}

int XmqHostServer::run()
{
    int ret{XMQNode::run()};

    if (Error_Code_Success == ret)
    {
        expire = ThreadPool().get_mutable_instance().createNew(
            boost::bind(&XmqHostServer::checkRegisterExpiredOfServiceThread, this));
    }
    
    return ret;
}

int XmqHostServer::stop()
{
    int ret{XMQNode::stop()};

    if (Error_Code_Success == ret && false == stopped)
    {
        stopped = true;
        Thread().join(expire);
        ThreadPool().get_mutable_instance().destroy(expire);
        XMQNode::removeConf(0xA1);
    }
    
    return ret;
}

void XmqHostServer::afterPolledDataNotification(
    const uint32_t id/* = nullptr*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
    const char* from/* = 0*/)
{
    //数据解析逻辑说明：
    // 1. 解析协议名称，如果是register则处理服务注册业务，否则到（2）；
    // 2. 解析主机名称，与服务名称对比是否一致，如果是则由服务执行业务处理， 否则到（3）；
    // 3. 在注册服务表中查找主机名称，如果存在，则执行转发消息，否则应答消息不可达错误。
    
    const std::string msg{(const char*)data, bytes};
    // fileLog.write(
    //     SeverityLevel::SEVERITY_LEVEL_INFO, 
    //     "Polled message from serivce = [ %s ], data = [ %s ].",  
    //     from, 
    //     msg.c_str());
    Url requestUrl;
    int ret{requestUrl.parse(msg)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.getProtocol().compare("register"))
        {
            processRegisterMessage(from, requestUrl);
        }
        else if (!requestUrl.getHost().compare(XMQHostID))
        {
            processRequestMessage(from, requestUrl);
        }
        else
        {
            const std::string to{requestUrl.getHost()};
            const unsigned long long timestamp{registeredServices.at(to)};

            if (0 < timestamp)
            {
                forwardCustomMessage(from, to, msg);
            }
            else
            {
                fileLog.write(
                    SeverityLevel::SEVERITY_LEVEL_WARNING, 
                    "Can not forward data from service = [ %s ] to service = [ %s ].",  
                    from, 
                    to.c_str());
            }
        }
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Parsed data from service = [ %s ] failed, result = [ %d ].",  
            from, 
            ret);
    }
}

void XmqHostServer::afterFetchOnlineStatusNotification(const bool online/* = false*/)
{}

void XmqHostServer::afterFetchServiceCapabilitiesNotification(
    const ServiceInfo* infos/* = nullptr*/, 
    const uint32_t number/* = 0*/)
{}

void XmqHostServer::checkRegisterExpiredOfServiceThread()
{
	while (!stopped)
	{
        XTime xt;
        const uint64_t startTickCount{xt.tickcount()};
        const std::vector<std::string> keies{registeredServices.keies()};

        for (int i = 0; i != keies.size(); ++i)
        {
            const uint64_t tick{registeredServices.at(keies[i])};
            const uint32_t diff{startTickCount > tick ? startTickCount - tick : tick - startTickCount};

            if (diff > 90000)
            {
                registeredServices.remove(keies[i]);
                fileLog.write(
                    SeverityLevel::SEVERITY_LEVEL_WARNING, 
                    "Remove service name = [ %s ] with expired = [ %ld ].", 
                    keies[i].c_str(), diff);
            }
        }
        
		xt.sleep(1000);
	}
}

void XmqHostServer::processRegisterMessage(const std::string from, Url& requestUrl)
{
    const std::string hostName{requestUrl.getHost()};
    int ret{!hostName.empty() ? Error_Code_Success : Error_Code_Bad_RequestUrl};

    if (Error_Code_Success == ret)
    {
        const std::vector<ParamItem> items{requestUrl.getParameters()};

        for(int i = 0; i != items.size(); ++i)
        {
            if(!items[i].key.compare("timestamp"))
            {
                registeredServices.replace(hostName, std::stoull(items[i].value));

                Url responseUrl;
                responseUrl.setProtocol(requestUrl.getProtocol());
                responseUrl.setHost(XMQHostID);
                responseUrl.addParameter(items[i].key, items[i].value);
                const std::string msg{responseUrl.encode()};
                ret = XMQNode::send(0xA1, msg.c_str(), msg.length(), from.c_str());
                break;
            }
        }
    }

    if (Error_Code_Success == ret)
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Process register message from serivce = [ %s ] successfully.",  
            hostName.c_str());
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING, 
            "Process register message from serivce = [ %s ] failed, result = [ %d ].",  
            hostName.c_str(),
            ret);
    }
}

void XmqHostServer::processRequestMessage(const std::string from, Url& requestUrl)
{
    const std::string protocol{requestUrl.getProtocol()};

    if (!protocol.compare("query"))
    {
        const std::vector<std::string> services{registeredServices.keies()};
        Url responseUrl;
        responseUrl.setProtocol(requestUrl.getProtocol());
        responseUrl.setHost(XMQHostID);
        for(int i = 0; i != services.size(); ++i)
        {
            responseUrl.addParameter("name", services[i]);
        }

        const std::string msg{responseUrl.encode()};
        int ret{XMQNode::send(0xA1, msg.c_str(), msg.length(), from.c_str())};

        if (Error_Code_Success == ret)
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Process query message from serivce = [ %s ] successfully.",  
                from.c_str());
        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Process query message with serivce = [ %s ] failed, result = [ %d ].",  
                from.c_str(), 
                ret);
        }
    }
    else
    {
        fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Parsed unknown data from service = [ %s ].",  
                from.c_str());
    }
}

void XmqHostServer::forwardCustomMessage(
    const std::string from, 
    const std::string to, 
    const std::string data)
{
    //转发消息在XMQ服务端将源地址追加到数据尾部
    //数据目的端解析尾部from字段可获取源地址以回复消息
    const std::string msg{data + "&from=" + from};
    int ret{
        XMQNode::send(0xA1, msg.c_str(), msg.length(), to.c_str())};

    if (Error_Code_Success == ret)
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Forward message from serivce = [ %s ] to service = [ %s ] with data = [ %s ] successfully.",  
            from.c_str(), 
            to.c_str(), 
            data.c_str());
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Forward message from serivce = [ %s ] to service = [ %s ] with data = [ %s ] failed, result = [ %d ].",  
            from.c_str(), 
            to.c_str(),  
            data.c_str(), 
            ret);
    }
}
