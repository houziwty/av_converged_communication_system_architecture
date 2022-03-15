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
	const uint32_t hwm/* = 3*/) 
	: XMQRole(conf, PolledDataCallback()), dataHWM{hwm}
{}

DataPub::~DataPub()
{}

int DataPub::run(ctx_t c/* = nullptr*/)
{
	int ret{c ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		so = Pub().bind(c, modeconf.port, dataHWM);

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

int DataPub::stop()
{
	int ret{XMQRole::stop()};

	if (Error_Code_Success == ret)
	{
		Pub().shutdown(so);
	}
	
	return ret;
}

int DataPub::send(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const char* /*name = nullptr*/)
{
	int ret{so ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		ret = (data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			msg.add(data, bytes);
			ret = msg.send(so);
		}
	}

	return ret;
}

void DataPub::pollDataThread()
{
	//数据发布没有数据接收
}

void DataPub::checkServiceOnlineStatusThread()
{
	//数据发布不检查模块在线状态
}
