#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "xmq_role.h"
using namespace module::network::xmq;

XMQRole::XMQRole(
	const XMQModeConf& conf, 
	PolledDataCallback callback) 
	: modeconf{conf}, so{nullptr}, poller{nullptr}, 
	stopped{true}, polledDataCallback{callback}
{}

XMQRole::~XMQRole()
{
	stop();
}

int XMQRole::run(ctx_t c/* = nullptr*/)
{
	int ret{c && stopped ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		stopped = false;
		poller = ThreadPool().get_mutable_instance().createNew(
			boost::bind(&XMQRole::pollDataThread, this));
		check = ThreadPool().get_mutable_instance().createNew(
			boost::bind(&XMQRole::checkServiceOnlineStatusThread, this));
		ret = poller && check ? Error_Code_Success : Error_Code_Bad_New_Thread;
	}
	
	return ret;
}

int XMQRole::stop()
{
	int ret{!stopped ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		stopped = true;
		Thread().join(poller);
		ThreadPool().get_mutable_instance().destroy(poller);
		Thread().join(check);
		ThreadPool().get_mutable_instance().destroy(check);
	}

	return ret;
}
