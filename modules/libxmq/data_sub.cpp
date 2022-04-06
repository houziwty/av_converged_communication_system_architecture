#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "xmq/sub.h"
#include "xmq/msg.h"
#include "data_sub.h"
using namespace module::network::xmq;

DataSub::DataSub(
	const XMQModeConf& conf, 
	PolledDataCallback callback) 
	: AsyncNode(conf, callback), sso{nullptr}
{}

DataSub::~DataSub()
{}

int DataSub::run(xctx c/* = nullptr*/)
{
	int ret{c ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		sso = Sub().connect(c, modeconf.ip, modeconf.port);

		if(sso)
		{
			ret = AsyncNode::run(c);
		}
		else
		{
			ret = Error_Code_Bad_New_Socket;
		}
	}

	return ret;
}

int DataSub::stop()
{
	int ret{AsyncNode::stop()};

	if (Error_Code_Success == ret)
	{
		Sub().shutdown(sso);
	}
	
	return ret;
}

int DataSub::send(
	const void* /*data = nullptr*/, 
	const uint64_t /*bytes = nullptr*/, 
	const char* /*id = 0*/)
{
	return Error_Code_Method_Not_Support;
}

void DataSub::pollDataThread()
{
	while(sso && !stopped)
	{
		zmq_pollitem_t pollitems[]{ { sso, 0, ZMQ_POLLIN, 0} };
		zmq_poll(pollitems, 1, 1);

		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			Msg msg;
			int ret {msg.recv(sso)};
			
			if (Error_Code_Success == ret)
			{
				//只读第一段数据
				const Any* first{msg.msg()};

				if (polledDataCallback)
				{
					polledDataCallback(modeconf.id, first->data(), first->bytes(), nullptr);
				}
			}
		}
	}
}
