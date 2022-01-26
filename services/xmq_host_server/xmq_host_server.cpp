#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "xmq_host_server.h"

XmqHostServer::XmqHostServer(FileLog& log) 
    : XMQNode(), fileLog{log}, stopped{false}, expire{nullptr}
{}

XmqHostServer::~XmqHostServer()
{}

int XmqHostServer::run()
{
    const std::string name{"xmq_host_server"};
    XMQModeConf conf{0};
    conf.id = 0xA1;
    conf.port = 60531;
    conf.type = XMQModeType::XMQ_MODE_TYPE_ROUTER;
    XMem().copy(name.c_str(), name.length(), conf.name, 128);

    int ret{XMQNode::addConf(conf)};

    if (Error_Code_Success == ret)
    {
        ret = XMQNode::run();

        if (Error_Code_Success == ret)
        {
            expire = ThreadPool().get_mutable_instance().createNew(
                boost::bind(&XmqHostServer::checkRegisterExpiredOfServiceThread, this));
        }
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
    const uint32_t uid/* = nullptr*/, 
	const char* name/* = 0*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
    //数据解析逻辑说明：
    // 1. 解析协议名称，如果是register则处理服务注册业务，否则到（2）；
    // 2. 解析主机名称，与服务名称对比是否一致，如果是则由服务执行业务处理， 否则到（3）；
    // 3. 在注册服务表中查找主机名称，如果存在，则执行转发消息，否则应答消息不可达错误。
    
    const std::string msg{(const char*)data, bytes};
    fileLog.write(
        SeverityLevel::SEVERITY_LEVEL_INFO, 
        "Polled message with serivce name = [ %s ] and data = [ %s ].",  
        name, 
        msg.c_str());
    Url requestUrl;
    int ret{requestUrl.parse(msg)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.getProtocol().compare("register"))
        {
            processRegisterMessage(name, requestUrl);
        }
        else if (!requestUrl.getHost().compare("xmq_host_server"))
        {
            processRequestMessage(name, requestUrl);
        }
        else
        {
            const std::string hostName{requestUrl.getHost()};
            const unsigned long long timestamp{registeredServices.at(hostName)};

            if (0 < timestamp)
            {
                forwardMessage(hostName, msg);
            }
            else
            {
                fileLog.write(
                    SeverityLevel::SEVERITY_LEVEL_WARNING, 
                    "Parsed unknown data from service name = [ %s ].",  
                    name);
            }
        }
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Parsed data from service name = [ %s ] failed, result = [ %d ].",  
            name, 
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
        const unsigned long long startTickCount{xt.tickcount()};
        const std::vector<std::string> keies{registeredServices.keies()};

        for (int i = 0; i != keies.size(); ++i)
        {
            const unsigned long long tick{registeredServices.at(keies[i])};
            const unsigned long long diff{startTickCount - tick};

            if (diff > 90000)
            {
                registeredServices.remove(keies[i]);
                fileLog.write(
                    SeverityLevel::SEVERITY_LEVEL_WARNING, "Remove service name = [ %s ] with expired = [ %ld ].",  keies[i].c_str(), diff);
            }
        }
        
		xt.sleep(1000);
	}
}

void XmqHostServer::processRegisterMessage(const std::string name, Url& requestUrl)
{
    const std::string hostName{requestUrl.getHost()};
    int ret{!hostName.compare(name) ? Error_Code_Success : Error_Code_Bad_RequestUrl};

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
                responseUrl.setHost("xmq_host_server");
                responseUrl.addParameter(items[i].key, items[i].value);
                const std::string msg{responseUrl.encode()};
                ret = XMQNode::send(0xA1, hostName.c_str(), msg.c_str(), msg.length());
                break;
            }
        }
    }

    if (Error_Code_Success == ret)
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Process register message with serivce name = [ %s ] successfully.",  
            hostName.c_str());
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_WARNING, 
            "Process register message with serivce name = [ %s ] failed, result = [ %d ].",  
            hostName.c_str(),
            ret);
    }
}

void XmqHostServer::processRequestMessage(const std::string name, Url& requestUrl)
{
    const std::string protocol{requestUrl.getProtocol()};

    if (!protocol.compare("query"))
    {
        const std::vector<std::string> services{registeredServices.keies()};
        Url responseUrl;
        responseUrl.setProtocol(requestUrl.getProtocol());
        responseUrl.setHost(name);
        for(int i = 0; i != services.size(); ++i)
        {
            responseUrl.addParameter("name", services[i]);
        }

        const std::string msg{responseUrl.encode()};
        int ret{XMQNode::send(0xA1, name.c_str(), msg.c_str(), msg.length())};

        if (Error_Code_Success == ret)
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Process query message with serivce name = [ %s ] successfully.",  
                name.c_str());
        }
        else
        {
            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_WARNING, 
                "Process query message with serivce name = [ %s ] failed, result = [ %d ].",  
                name.c_str(), 
                ret);
        }
    }
}

void XmqHostServer::forwardMessage(const std::string name, const std::string data)
{
    int ret{
        XMQNode::send(0xA1, name.c_str(), data.c_str(), data.length())};

    if (Error_Code_Success == ret)
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Forward message to serivce name = [ %s ] and data = [ %s ] successfully.",  
            name.c_str(), 
            data.c_str());
    }
    else
    {
        fileLog.write(
            SeverityLevel::SEVERITY_LEVEL_ERROR, 
            "Forward message to serivce name = [ %s ] and data = [ %s ] failed, result = [ %d ].",  
            name.c_str(), 
            data.c_str(), 
            ret);
    }
}
