#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/format.hpp"
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "utils/url/url.h"
using namespace framework::utils::url;
#include "network/xmq/worker_deal.h"
using namespace framework::network::xmq;

WorkerDeal::WorkerDeal() : thread{nullptr}, stopped{ false }
{}

WorkerDeal::~WorkerDeal()
{
	stop();
}

int WorkerDeal::start(
	const std::string appid, 
	const std::string xmqid, 
	const std::string ip, 
	const unsigned short port /*= 0*/)
{
	if (workerPtr)
	{
		return Error_Code_Object_Existed;
	}
	
	int ret{ !appid.empty() &&  !appid.empty() && !ip.empty() && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		WorkerPtr ptr{
			boost::make_shared<Worker>(
				boost::bind(&WorkerDeal::afterWorkerPolledDataHandler, this, _1))};

		if(ptr && Error_Code_Success == ptr->connect(appid, ip, port))
		{
			workerPtr.swap(ptr);
			thread = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&WorkerDeal::pollDataFromWorkerThread, this));
		}

		ret = (workerPtr ? keepalive(appid, xmqid) : Error_Code_Bad_New_Object);
	}

	return ret;
}

int WorkerDeal::stop()
{
	int ret{workerPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		stopped = true;
		Thread().join(thread);
		ThreadPool().get_mutable_instance().destroy(thread);
		workerPtr.reset();
	}

	return ret;
}

int WorkerDeal::send(const std::string data)
{
	int ret{ workerPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		ret = workerPtr->send(data);
	}

	return ret;
}

int WorkerDeal::keepalive(
	const std::string appid, 
	const std::string xmqid,  
	const int interval/* = 30*/)
{	
	int ret{workerPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		ThreadPool().get_mutable_instance().createNew(
			boost::bind(&WorkerDeal::keepAliveWorkerThread, this, appid, xmqid, interval));
	}

	return ret;
}

void WorkerDeal::pollDataFromWorkerThread()
{
	while (workerPtr && !stopped)
	{
		workerPtr->poll();
	}
}

void WorkerDeal::keepAliveWorkerThread(
	const std::string appid, 
	const std::string xmqid,  
	const int interval/* = 30*/)
{
	int sequence{1};
	unsigned long long lastTickCout{0};
	XTime xt;

	while (!stopped)
	{
		unsigned long long currentTickCount{xt.tickcount()};

		if(currentTickCount - lastTickCout > interval * 1000)
		{
			//Register and heartbeat
			Url url;
			url.setProtocol("register");
			url.setHost(xmqid);
			url.addParameter("tick", (boost::format("%ld") % currentTickCount).str());
			url.addParameter("sequence", (boost::format("%d") % sequence++).str());
			const std::string data{url.encode()};

			if(!data.empty())
			{
				send(data);
			}

			lastTickCout = currentTickCount;
		}

		xt.sleep(500);
	}
}
