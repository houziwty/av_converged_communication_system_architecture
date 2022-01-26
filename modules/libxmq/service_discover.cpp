#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "xmq/router.h"
#include "xmq/msg.h"
#include "service_discover.h"
using namespace module::network::xmq;

ServiceDiscover::ServiceDiscover(
	const XMQModeConf& conf, 
	PolledDataCallback pollcb) 
	: XMQRole(conf, pollcb)
{}

ServiceDiscover::~ServiceDiscover()
{}

int ServiceDiscover::run(ctx_t c/* = nullptr*/)
{
	int ret{c && stopped ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		so = Router().bind(c, modeconf.port);

		if (so)
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

int ServiceDiscover::stop()
{
	int ret{XMQRole::stop()};

	if (Error_Code_Success == ret)
	{
		Router().shutdown(so);
	}
	
	return ret;
}

int ServiceDiscover::send(
	const char* name/* = nullptr*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{so ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		ret = (name && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			const std::string id{name};
			Msg msg;
			msg.append(id.c_str(), id.length());
			msg.append("", 0);
			msg.append(data, bytes);
			ret = msg.send(so);
		}
	}

	return ret;
}

void ServiceDiscover::pollDataThread()
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
				//读取第一段和第三段数据
 				const Message* first{ msg.msg() };
				const Message* third{ msg.msg(2) };

				if (polledDataCallback)
				{
					polledDataCallback(modeconf.id, (const char*)first->data, third->data, third->bytes);
				}
			}
		}
	}
}

void ServiceDiscover::checkServiceOnlineStatusThread()
{
	//服务发现不检查模块在线状态
}
