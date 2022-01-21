#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "error_code.h"
#include "xmq/router.h"
#include "service_discover.h"
using namespace module::network::xmq;

ServiceDiscover::ServiceDiscover(const XMQModeConf& conf) 
	: XMQRole(), modeconf{conf}, so{nullptr}
{}

ServiceDiscover::~ServiceDiscover()
{}

int ServiceDiscover::run(ctx_t c/* = nullptr*/)
{
	int ret{c && stopped ? Error_Code_Success ? Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		so = Router().bind(c, modeconf.port);

		if (so)
		{
			thread = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&ServiceDiscover::checkServiceOnlineStatusThread, this));
			ret = XMQRole::run(c);
		}
		else
		{
			ret = Error_Code_Bad_New_Socket;
		}
	}
	
	return ret;

	
	
	return so ? Error_Code_Success : Error_Code_Bad_Operate_Bind;
}

int Switcher::send(
	const void* uid/* = nullptr*/, 
	const int uid_bytes/* = 0*/, 
	const void* data/* = nullptr*/, 
	const int data_bytes/* = 0*/)
{
	int ret{ctx && router ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		ret = (uid && 0 < uid_bytes && data && 0 < data_bytes ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			msg.append(uid, uid_bytes);
			msg.append("", 0);
			msg.append(data, data_bytes);
			ret = msg.send(router);
		}
	}

	return ret;
}

int ServiceDiscover::pollDataThread()
{
	int ret{router ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret) 
	{
		zmq_pollitem_t pollitems[]{ { router, 0, ZMQ_POLLIN, 0} };
		zmq_poll(pollitems, 1, 1);

		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			Msg msg;
			ret = msg.recv(router);
			if (Error_Code_Success == ret)
			{
				//只读第一和第三段数据
 				const Message* header{ msg.msg() };
				const Message* body{ msg.msg(2) };
				
				if (handler)
				{
					handler(header->data, header->bytes, body->data, body->bytes);
				}
			}
		}
	}

	return ret;
}
