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
#include "network/xmq/worker_model.h"
using namespace framework::network::xmq;

WorkerModel::WorkerModel() 
	: pollDataThread{nullptr}, keepaliveThread{nullptr}, stopped{ false }
{}

WorkerModel::~WorkerModel()
{
	stop();
}

int WorkerModel::start(
	const std::string name, 
	const std::string ip, 
	const unsigned short port /*= 0*/)
{
	if (workerPtr)
	{
		return Error_Code_Object_Existed;
	}
	
	int ret{!name.empty() && !ip.empty() && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		WorkerPtr ptr{
			boost::make_shared<Worker>(
				boost::bind(&WorkerModel::afterWorkerPolledDataHandler, this, _1))};

		if(ptr && Error_Code_Success == ptr->connect(name, ip, port))
		{
			serviceName = name;
			workerPtr.swap(ptr);
			pollDataThread = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&WorkerModel::pollDataFromWorkerThread, this));
			keepaliveThread = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&WorkerModel::keepAliveWorkerThread, this));
		}
	}

	return ret;
}

int WorkerModel::stop()
{
	int ret{workerPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		stopped = true;
		Thread().join(pollDataThread);
		Thread().join(keepaliveThread);
		ThreadPool().get_mutable_instance().destroy(pollDataThread);
		ThreadPool().get_mutable_instance().destroy(keepaliveThread);
		workerPtr.reset();
	}

	return ret;
}

int WorkerModel::send(const std::string data)
{
	int ret{ workerPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		ret = workerPtr->send(data);
	}

	return ret;
}

void WorkerModel::pollDataFromWorkerThread()
{
	while (workerPtr && !stopped)
	{
		workerPtr->poll();
	}
}

void WorkerModel::keepAliveWorkerThread()
{
	int sequence{1};
	unsigned long long lastTickCout{0};
	XTime xt;

	while (!stopped)
	{
		unsigned long long currentTickCount{xt.tickcount()};

		if(currentTickCount - lastTickCout > 30000)
		{
			//Register and heartbeat
			Url url;
			url.setProtocol("register");
			url.setHost(serviceName);
			url.addParameter("timestamp", (boost::format("%ld") % currentTickCount).str());
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
