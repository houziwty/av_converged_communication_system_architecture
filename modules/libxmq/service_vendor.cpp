#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "boost/format.hpp"
#include "error_code.h"
#include "thread/thread.h"
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "time/xtime.h"
using namespace framework::utils::time;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "url/url.h"
using namespace framework::utils::data;
#include "xmq/dealer.h"
#include "xmq/msg.h"
#include "service_vendor.h"
using namespace module::network::xmq;

ServiceVendor::ServiceVendor(
	const XMQModeConf& conf, 
	PolledDataCallback poll, 
	CheckOnlineStatusCallback check, 
	ServiceCapabilitiesNotificationCallback capabilities) 
	: AsyncNode(conf, poll), registerResponseTimetamp{XTime().tickcount()}, 
	online{false}, checker_t{nullptr}, checkOnlineStatusCallback{check}, 
	serviceCapabilitiesNotificationCallback{capabilities}
{}

ServiceVendor::~ServiceVendor()
{}

int ServiceVendor::run(xctx c/* = nullptr*/)
{
	int ret{c && stopped ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		dso = Dealer().connect(c, modeconf.name, modeconf.ip, modeconf.port);

		if(dso && Error_Code_Success == AsyncNode::run(c))
		{
			checker_t = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&ServiceVendor::checkServiceOnlineStatusThread, this));
		}
		else
		{
			stop();
			ret = Error_Code_Bad_New_Socket;
		}
	}

	return ret;
}

int ServiceVendor::stop()
{
	int ret{AsyncNode::stop()};

	if (Error_Code_Success == ret)
	{
		Thread().join(checker_t);
		ThreadPool().get_mutable_instance().destroy(checker_t);
		Dealer().shutdown(dso);
	}
	
	return ret;
}

int ServiceVendor::send(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const char* /*id = nullptr*/)
{
	int ret{dso ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		ret = (data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			msg.add("", 0);
			msg.add(data, bytes);			
			ret = msg.send(dso);
		}
	}

	return ret;
}

void ServiceVendor::pollDataThread()
{
	while(dso && !stopped)
	{
		zmq_pollitem_t pollitems[]{ { dso, 0, ZMQ_POLLIN, 0} };
		zmq_poll(pollitems, 1, 1);

		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			Msg msg;
			int ret{msg.recv(dso)};

			if (Error_Code_Success == ret)
			{
				//读取第二段数据
				const Any* second{ msg.msg(1) };
				Url url;

				if (Error_Code_Success == url.parse(second->data(), second->bytes()))
				{
					const std::string protocol{url.proto()};
					if (!protocol.compare("register"))
					{
						processRegisterResponseMessage(&url);
					}
					else if (!protocol.compare("query"))
					{
						processQueryResponseMessage(&url);
					}
					else
					{
						processForwardCustomMessage(second->data(), second->bytes());
					}
				}
			}
		}
	}
}

void ServiceVendor::checkServiceOnlineStatusThread()
{
	int sequence{1};
	uint64_t lastTickCout{0};
	XTime xt;

	while (!stopped)
	{
		uint64_t currentTickCount{xt.tickcount()};

		if(currentTickCount - lastTickCout > 30000)
		{
			//Register and heartbeat
			const std::string data{
				(boost::format("register://%s?timestamp=%lld&sequence=%lld") % modeconf.name % currentTickCount % sequence++).str()};

			if(!data.empty())
			{
				send(data.c_str(), data.length());
			}

			lastTickCout = currentTickCount;
		}

		if(checkOnlineStatusCallback && currentTickCount - registerResponseTimetamp >= 90000 && online)
		{
			online = false;
			checkOnlineStatusCallback(online);
		}

		xt.sleep(1000);
	}
}

void ServiceVendor::processRegisterResponseMessage(void* url /* = nullptr */)
{
    const std::vector<Parameter>& items{((Url*)url)->parameters()};

    for(int i = 0; i != items.size(); ++i)
    {
        if(!items[i].key.compare("timestamp"))
        {
            registerResponseTimetamp = std::stoull(items[i].value);

			if(checkOnlineStatusCallback && XTime().tickcount() - registerResponseTimetamp < 90000 && !online)
			{
				online = true;
				checkOnlineStatusCallback(online);
			}

			if (online)
			{
				//在线就获取服务信息
				const std::string msg{(boost::format("query://%s") % XMQHostID).str()};
            	send(msg.c_str(), msg.length());
			}
			
            break;
        }
    }
}

void ServiceVendor::processQueryResponseMessage(void* url /* = nullptr */)
{
    ServiceInfo* infos{nullptr};
    const std::vector<Parameter>& items{((Url*)url)->parameters()};
    const std::size_t number{items.size()};

    if (0 < number)
    {
        infos = new(std::nothrow) ServiceInfo[number];
		memset(infos, 0, number * sizeof(ServiceInfo));
    }

    for(int i = 0; i != number; ++i)
    {
        if(!items[i].key.compare("name"))
        {
			XMem().copy((const void*)items[i].value.c_str(), items[i].value.length(), (void*)infos[i].name, 128);
        }
    }

	if (serviceCapabilitiesNotificationCallback)
	{
		serviceCapabilitiesNotificationCallback(infos, static_cast<uint32_t>(number));
	}
	
    boost::checked_array_delete(infos);
}

void ServiceVendor::processForwardCustomMessage(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	if (data && 0 < bytes)
	{
		const std::string str{(const char*)data, bytes};

		//解析经XMQ转发消息时追加的from字段
		std::size_t pos{str.find_last_of('&')};
		const std::string msg{str.substr(0, pos)};
		const std::string appendix{str.substr(pos + 1, str.length())};
		pos = appendix.find_last_of('=');
		const std::string from{appendix.substr(pos + 1, appendix.length())};

		if (polledDataCallback)
		{
			polledDataCallback(modeconf.id, msg.c_str(), msg.length(), from.c_str());
		}
	}
}
