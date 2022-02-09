#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "boost/format.hpp"
#include "error_code.h"
#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "xmq/dealer.h"
#include "xmq/msg.h"
#include "service_vendor.h"
using namespace module::network::xmq;

ServiceVendor::ServiceVendor(
	const XMQModeConf& conf, 
	PolledDataCallback pollcb, 
	CheckOnlineStatusCallback checkcb, 
	ServiceCapabilitiesNotificationCallback capabilitiescb) 
	: XMQRole(conf, pollcb), registerResponseTimetamp{XTime().tickcount()}, 
	online{false}, checkOnlineStatusCallback{checkcb}, 
	serviceCapabilitiesNotificationCallback{capabilitiescb}
{}

ServiceVendor::~ServiceVendor()
{}

int ServiceVendor::run(ctx_t c/* = nullptr*/)
{
	int ret{c && stopped ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		so = Dealer().connect(modeconf.name, modeconf.ip, modeconf.port, c);

		if(so)
		{
			ret = XMQRole::run(c);
		}
		else
		{
			ret = Error_Code_Bad_New_Socket;
		}
	}

	return ret;
}

int ServiceVendor::stop()
{
	int ret{XMQRole::stop()};

	if (Error_Code_Success == ret)
	{
		Dealer().shutdown(so);
	}
	
	return ret;
}

int ServiceVendor::send(
	const char* /*name*//* = nullptr*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{so ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		ret = (data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			msg.append("", 0);
			msg.append(data, bytes);
			ret = msg.send(so);
		}
	}

	return ret;
}

void ServiceVendor::pollDataThread()
{
	while(so && !stopped)
	{
		zmq_pollitem_t pollitems[]{ { so, 0, ZMQ_POLLIN, 0} };
		zmq_poll(pollitems, 1, 1);

		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			Msg msg;
			int ret{msg.recv(so)};

			if (Error_Code_Success == ret)
			{
				//读取第二段数据
				const Message* second{ msg.msg(1) };
				const std::string data{
					(const char*)second->data, 
					static_cast<unsigned int>(second->bytes)};
				Url url;

				if (Error_Code_Success == url.parse(data))
				{
					const std::string protocol{url.getProtocol()};
					if (!protocol.compare("register"))
					{
						processRegisterResponseMessage(url);
					}
					else if (!protocol.compare("query"))
					{
						processQueryResponseMessage(url);
					}
					else if (polledDataCallback)
					{
						polledDataCallback(modeconf.id, nullptr, second->data, second->bytes);
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
			Url url;
			url.setProtocol("register");
			url.setHost(modeconf.name);
			url.addParameter("timestamp", (boost::format("%ld") % currentTickCount).str());
			url.addParameter("sequence", (boost::format("%d") % sequence++).str());
			const std::string data{url.encode()};

			if(!data.empty())
			{
				send(nullptr, data.c_str(), data.length());
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

void ServiceVendor::processRegisterResponseMessage(Url& url)
{
    const std::vector<ParamItem> items{url.getParameters()};

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
				const std::string msg{(boost::format("query://%s") % url.getHost()).str()};
            	send(nullptr, msg.c_str(), msg.length());
			}
			
            break;
        }
    }
}

void ServiceVendor::processQueryResponseMessage(Url& url)
{
    ServiceInfo* infos{nullptr};
    const std::vector<ParamItem> items{url.getParameters()};
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
			XMem().copy(items[i].value.c_str(), items[i].value.length(), infos[i].name, 128);
        }
    }

	if (serviceCapabilitiesNotificationCallback)
	{
		serviceCapabilitiesNotificationCallback(infos, static_cast<uint32_t>(number));
	}
	
    boost::checked_array_delete(infos);
}
