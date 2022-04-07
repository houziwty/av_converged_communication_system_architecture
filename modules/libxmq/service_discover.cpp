#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "hardware/cpu.h"
using namespace framework::utils::hardware;
#include "thread/thread.h"
#include "thread/thread_pool.h"
using namespace framework::utils::thread;
#include "xmq/router.h"
#include "xmq/dealer.h"
#include "xmq/msg.h"
#include "service_discover.h"
using namespace module::network::xmq;

ServiceDiscover::ServiceDiscover(
	const XMQModeConf& conf, 
	PolledDataCallback callback) 
	: AsyncNode(conf, callback), taskNumber{(uint8_t)Cpu().getCount()}
{
	memset(tasks, 0, 32 * sizeof(xthread));
}

ServiceDiscover::~ServiceDiscover()
{}

int ServiceDiscover::run(xctx c/* = nullptr*/)
{
	int ret{c && stopped ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		rso = Router().bind(c, modeconf.port);
		dso = Dealer().bind(c, taskName);

		if (rso && dso && Error_Code_Success == AsyncNode::run(c))
		{
			ret = createTaskThread(c);
		}
		else
		{
			stop();
			ret = Error_Code_Bad_New_Socket;
		}
	}
	
	return ret;
}

int ServiceDiscover::stop()
{
	int ret{AsyncNode::stop()};

	if (Error_Code_Success == ret)
	{
		destroyTaskThread();
		Router().shutdown(rso);
		Dealer().shutdown(dso);
	}
	
	return ret;
}

int ServiceDiscover::send(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const char* id/* = nullptr*/)
{
	int ret{rso ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		ret = (data && 0 < bytes && id ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			const std::string to{id};
			Msg msg;
			msg.add(to.c_str(), to.length());
			msg.add("", 0);
			msg.add(data, bytes);			
			ret = msg.send(rso);
		}
	}

	return ret;
}

void ServiceDiscover::pollDataThread()
{
	zmq_pollitem_t pollers[]{ 
		{ rso, 0, ZMQ_POLLIN, 0}, 
		{ dso, 0, ZMQ_POLLIN, 0} };

	while(rso && dso && !stopped) 
	{
		zmq_poll(pollers, 2, 1);
		Msg msg;

		if (pollers[0].revents & ZMQ_POLLIN)
		{
			msg.forward(rso, dso);
		}
		else if (pollers[1].revents & ZMQ_POLLIN)
		{
			//读取第一段和第三段数据
			const Any* first{ msg.msg() };
			const Any* third{ msg.msg(2) };

			if (polledDataCallback)
			{
				//避免Windows下数据超界产生的错误
				const std::string from{(const char*)first->data(), first->bytes()};
				polledDataCallback(modeconf.id, third->data(), third->bytes(), from.c_str());
			}
		}
	}
}

int ServiceDiscover::createTaskThread(xctx c/* = nullptr*/)
{
	int ret{0 < taskNumber ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		for(int i = 0; i != taskNumber; ++i)
		{
			tasks[i] = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&ServiceDiscover::processTaskThread, this, c));
		}
	}

	return ret;
}

int ServiceDiscover::destroyTaskThread()
{
	int ret{0 < taskNumber ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		for(int i = 0; i != taskNumber; ++i)
		{
			ThreadPool().get_mutable_instance().destroy(tasks[i]);
		}
	}

	return ret;
}

void ServiceDiscover::processTaskThread(xctx c/* = nullptr*/)
{
	if(c)
	{
		xsocket s{Dealer().connect_task(c, taskName)};
		zmq_pollitem_t poller[]{ { s, 0, ZMQ_POLLIN, 0} };

		while(!stopped)
		{
			zmq_poll(poller, 1, 1);

			if (poller[0].revents & ZMQ_POLLIN)
			{
				Msg msg;
				int ret{msg.recv(s)};
				//读取第一段和第三段数据
				const Any* first{ msg.msg() };
				const Any* third{ msg.msg(2) };

				if (polledDataCallback)
				{
					//避免Windows下数据超界产生的错误
					const std::string from{(const char*)first->data(), first->bytes()};
					polledDataCallback(modeconf.id, third->data(), third->bytes(), from.c_str());
				}
			}
		}
	}
}
