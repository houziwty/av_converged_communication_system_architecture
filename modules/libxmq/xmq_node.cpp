#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "error_code.h"
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "xmq_node.h"
using namespace module::network::xmq;

XMQNode::XMQNode(PolledDataCallback callback) 
	: poller{nullptr}, stopped{false}, polledDataCallback{callback}
{}

XMQNode::~XMQNode()
{
	stop();
}

int XMQNode::run(
	const XMQNodeConf& conf, 
	void* ctx/* = nullptr*/)
{
	int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		poller = ThreadPool().get_mutable_instance().createNew(
			boost::bind(&XMQNode::pollDataThread, this));
		ret = (poller ? Error_Code_Success : Error_Code_Bad_New_Thread);
	}
	
	return ret;
}

int XMQNode::stop()
{
	int ret{!stopped ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		stopped = true;
		ThreadPool().get_mutable_instance().destroy(poller);
	}

	return ret;
}
