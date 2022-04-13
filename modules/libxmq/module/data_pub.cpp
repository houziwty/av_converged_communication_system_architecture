#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "core/pub.h"
#include "core/sock.h"
#include "data_pub.h"
using namespace module::network::xmq;

DataPub::DataPub() 
	: XMQNode(PolledDataCallback()), pso{nullptr}
{}

DataPub::~DataPub()
{}

int DataPub::run(
	const XMQNodeConf& conf, 
	void* ctx/* = nullptr*/)
{
	int ret{ctx && 0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		pso = Pub().bind(ctx, conf.port, conf.opt.hwm);

		if(pso)
		{
			ret = XMQNode::run(conf, ctx);
		}
	}

	return ret;
}

int DataPub::stop()
{
	int ret{XMQNode::stop()};

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
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Operate_Failure};
	int sentbytes{0};

	if(pso && Error_Code_Success == ret)
	{
		//只发数据
		sentbytes = Sock().send(pso, data, bytes, false);
	}

	return sentbytes;
}

void DataPub::pollDataThread()
{
	//数据发布没有数据接收
}
