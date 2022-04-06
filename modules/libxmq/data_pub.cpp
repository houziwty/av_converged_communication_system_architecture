#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "xmq/pub.h"
#include "xmq/msg.h"
#include "data_pub.h"
using namespace module::network::xmq;

DataPub::DataPub(
	const XMQModeConf& conf, 
	const uint32_t number/* = 3*/) 
	: AsyncNode(conf, PolledDataCallback()), hwm{number}, pso{nullptr}
{}

DataPub::~DataPub()
{}

int DataPub::run(xctx c/* = nullptr*/)
{
	int ret{c ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		pso = Pub().bind(c, modeconf.port, hwm);

		if(pso)
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

int DataPub::stop()
{
	int ret{AsyncNode::stop()};

	if (Error_Code_Success == ret)
	{
		Pub().shutdown(pso);
	}
	
	return ret;
}

int DataPub::send(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const char* /*name = nullptr*/)
{
	int ret{pso ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		ret = (data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			msg.add(data, bytes);
			ret = msg.send(pso);
		}
	}

	return ret;
}

void DataPub::pollDataThread()
{
	//数据发布没有数据接收
}
