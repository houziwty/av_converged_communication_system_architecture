#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "zmq.h"
#include "error_code.h"
#include "core/router.h"
#include "core/dealer.h"
#include "core/sock.h"
#include "data_dispatcher.h"
using namespace module::network::xmq;

DataDispatcher::DataDispatcher() 
	: XMQNode(PolledDataCallback()), rso{nullptr}, dso{nullptr}
{}

DataDispatcher::~DataDispatcher()
{}

int DataDispatcher::run(
	const XMQNodeConf& conf, 
	void* ctx/* = nullptr*/)
{
	int ret{ctx && 0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		rso = Router().bind(ctx, conf.port, conf.opt.hwm);
		dso = Dealer().bind(ctx, "inproc://service_dispatcher_task");

		if (rso && dso)
		{
			ret = XMQNode::run(conf, ctx);
		}
	}
	
	return ret;
}

int DataDispatcher::stop()
{
	int ret{XMQNode::stop()};

	if (Error_Code_Success == ret)
	{
		Router().shutdown(rso);
		Dealer().shutdown(dso);
	}
	
	return ret;
}

int DataDispatcher::send(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const char* name/* = nullptr*/)
{
	return Error_Code_Method_Not_Support;
}

void DataDispatcher::pollDataThread()
{
	if (rso && dso)
	{
		zmq_proxy(rso, dso, nullptr);
	}
}
