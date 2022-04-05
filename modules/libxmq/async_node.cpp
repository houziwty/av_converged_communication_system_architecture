#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "error_code.h"
#include "thread/thread.h"
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "async_node.h"
using namespace module::network::xmq;

AsyncNode::AsyncNode(
	const XMQModeConf& conf, 
	PolledDataCallback callback) 
	: modeconf{conf}, poller_t{nullptr}, stopped{true}, polledDataCallback{callback}
{}

AsyncNode::~AsyncNode()
{
	stop();
}

int AsyncNode::run(xctx c/* = nullptr*/)
{
	int ret{c && stopped ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		stopped = false;
		poller_t = ThreadPool().get_mutable_instance().createNew(
			boost::bind(&AsyncNode::pollDataThread, this));
		ret = (poller_t ? Error_Code_Success : Error_Code_Bad_New_Thread);
	}
	
	return ret;
}

int AsyncNode::stop()
{
	int ret{!stopped ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		stopped = true;
		Thread().join(poller_t);
		ThreadPool().get_mutable_instance().destroy(poller_t);
	}

	return ret;
}
