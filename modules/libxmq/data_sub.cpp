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
	PolledDataCallback pollcb) 
	: XMQRole(conf, pollcb)
{}

DataSub::~DataSub()
{}

int DataSub::run(ctx_t c/* = nullptr*/)
{
	int ret{c ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		so = Sub().connect(modeconf.ip, modeconf.port, c);

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

int DataSub::stop()
{
	int ret{XMQRole::stop()};

	if (Error_Code_Success == ret)
	{
		Sub().shutdown(so);
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
	while(so && !stopped)
	{
		zmq_pollitem_t pollitems[]{ { so, 0, ZMQ_POLLIN, 0} };
		zmq_poll(pollitems, 1, 1);

		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			Msg msg;
			int ret {msg.recv(so)};
			
			if (Error_Code_Success == ret)
			{
				//只读第一段数据
				const Message* first{msg.msg()};

				if (polledDataCallback)
				{
					polledDataCallback(modeconf.id, first->data, first->bytes, nullptr);
				}
			}
		}
	}
}

void DataSub::checkServiceOnlineStatusThread()
{
	//数据订阅不检查模块在线状态
}
