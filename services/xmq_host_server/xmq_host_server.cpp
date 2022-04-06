#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/format.hpp"
#include "error_code.h"
#include "thread/thread.h"
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "time/xtime.h"
using namespace framework::utils::time;
#include "xmq_host_server.h"

XmqHostServer::XmqHostServer(const XMQModeConf& conf, FileLog& flog)
    : XMQNode(), modeconf{ conf }, log{flog}, stopped{ false }, expire{ nullptr }
{}

XmqHostServer::~XmqHostServer()
{}

int XmqHostServer::run()
{
    int ret{XMQNode::run()};

    if (Error_Code_Success == ret)
    {
        logid.append(XMQHostID).append("_log");
        expire = ThreadPool().get_mutable_instance().createNew(
            boost::bind(&XmqHostServer::checkRegisterExpiredOfServiceThread, this));

//         const std::string log{
//             (boost::format(
//                 "info://%s?command=add&severity=0&log=Run xmq host server with name = [ %s ], local_port = [ %d ], id = [ %x ], type = [ %d ].") 
//                 % logid % modeconf.name % modeconf.port % modeconf.id % static_cast<int>(modeconf.type)).str()};
//         XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
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
        XMQNode::removeConf(modeconf.id);
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
    
    Url requestUrl;
    int ret{requestUrl.parse(data, bytes)};

    if(Error_Code_Success == ret)
    {
        if (!requestUrl.proto().compare("register"))
        {
            processRegisterMessage(from, requestUrl);
        }
        else if (!requestUrl.host().compare(XMQHostID))
        {
            processRequestMessage(from, requestUrl);
        }
        else
        {
            const std::string to{requestUrl.host()};
            const unsigned long long timestamp{registeredServices.at(to)};

            if (0 < timestamp)
            {
                // const std::string msg{(const char*)data, bytes};
                // forwardCustomMessage(from, to, msg);
            }
            else
            {
//                 const std::string log{
//                     (boost::format(
//                         "info://%s?command=add&severity=1&log=Can not forward data from service = [ %s ] to service = [ %s ] with url [ %s ].") 
//                         % logid % from % to % msg).str()};
//                 XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());
            }
        }
    }
    else
    {
//         const std::string log{
//             (boost::format(
//                 "info://%s?command=add&severity=2&log=Parsed url from service = [ %s ] failed, result = [ %d ].") 
//                 % logid % from % ret).str()};
//         XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());

        const std::string text{ (const char*)data, bytes };
		log.write(
			SeverityLevel::SEVERITY_LEVEL_WARNING,
			"Parse request message [ %s ] failed, result [ %d ].",
			text.c_str(), ret);
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
            const uint64_t diff{startTickCount > tick ? startTickCount - tick : tick - startTickCount};

            if (diff > 90000)
            {
                registeredServices.remove(keies[i]);
//                 const std::string log{
//                     (boost::format(
//                         "info://%s?command=add&severity=1&log=Remove service name = [ %s ] while time has expired = [ %ld ].") 
//                         % logid % keies[i] % diff).str()};
//                 XMQNode::send(modeconf.id, log.c_str(), log.length(), logid.c_str());

				log.write(
					SeverityLevel::SEVERITY_LEVEL_WARNING,
					"Remove service [ %s ] while time out, diff [ %lld ].",
                    keies[i].c_str(), diff);
            }
        }
        
		xt.sleep(1000);
	}
}

void XmqHostServer::processRegisterMessage(const std::string from, Url& requestUrl)
{
    const std::string hostName{requestUrl.host()};
    int ret{!hostName.empty() ? Error_Code_Success : Error_Code_Bad_RequestUrl};

    if (Error_Code_Success == ret)
    {
        const std::vector<Parameter> items{requestUrl.parameters()};

        for(int i = 0; i != items.size(); ++i)
        {
            if(!items[i].key.compare("timestamp"))
            {
                registeredServices.replace(hostName, std::stoull(items[i].value));
                const std::string msg{
                    (boost::format("register://%s?timestamp=%lld") % XMQHostID % items[i].value).str()};
                ret = XMQNode::send(modeconf.id, msg.c_str(), msg.length(), from.c_str());

				if (Error_Code_Success == ret)
				{
					log.write(
						SeverityLevel::SEVERITY_LEVEL_INFO,
						"Send response message of register [ %s ] successfully.",
						msg.c_str());
				}
				else
				{
					log.write(
						SeverityLevel::SEVERITY_LEVEL_ERROR,
						"Send response message of register [ %s ] failed, result [ %d ].",
						msg.c_str(), ret);
				}

                break;
            }
        }
    }
}

void XmqHostServer::processRequestMessage(const std::string from, Url& requestUrl)
{
    const std::string protocol{requestUrl.proto()};

    if (!protocol.compare("query"))
    {
        const std::vector<std::string> names{registeredServices.keies()};
        std::string parameters;
        for(int i = 0; i != names.size(); ++i)
        {
            if (parameters.empty())
            {
                parameters += "name=";
            }
            else
            {
                parameters += "&name=";
            }

            parameters += names[i];
        }

        const std::string msg{
            (boost::format("query://%s?%s") % XMQHostID % parameters).str()};
        int ret{ XMQNode::send(modeconf.id, msg.c_str(), msg.length(), from.c_str()) };

		if (Error_Code_Success == ret)
		{
			log.write(
				SeverityLevel::SEVERITY_LEVEL_INFO,
				"Send response message of query [ %s ] successfully.",
				msg.c_str());
		}
		else
		{
			log.write(
				SeverityLevel::SEVERITY_LEVEL_ERROR,
				"Send response message of query [ %s ] failed, result [ %d ].",
				msg.c_str(), ret);
		}
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
    int ret{ XMQNode::send(modeconf.id, msg.c_str(), msg.length(), to.c_str()) };

	if (Error_Code_Success == ret)
	{
		// log.write(
		// 	SeverityLevel::SEVERITY_LEVEL_INFO,
		// 	"Send forwarding message [ %s ] successfully.",
		// 	msg.c_str());
	}
	else
	{
		log.write(
			SeverityLevel::SEVERITY_LEVEL_ERROR,
			"Send forwarding message [ %s ] failed, result [ %d ].",
			msg.c_str(), ret);
	}
}
