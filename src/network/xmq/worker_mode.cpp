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
#include "utils/memory/xstr.h"
using namespace framework::utils::memory;
#include "network/xmq/worker_mode.h"
using namespace framework::network::xmq;

WorkerMode::WorkerMode() 
	: pollDataThread{nullptr}, keepaliveThread{nullptr}, stopped{ false }
{}

WorkerMode::~WorkerMode()
{
	stop();
}

int WorkerMode::start(
	const void* name/* = nullptr*/, 
	const int bytes/* = 0*/,  
	const char* ip/* = nullptr*/, 
	const unsigned short port/* = 0*/)
{
	if (workerPtr)
	{
		return Error_Code_Object_Existed;
	}
	
	int ret{name && 0 < bytes && ip && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		WorkerPtr ptr{
			boost::make_shared<Worker>(
				boost::bind(&WorkerMode::afterWorkerPolledDataHandler, this, _1, _2))};

		if(ptr && Error_Code_Success == ptr->connect(name, bytes, ip, port))
		{
			const std::string worker_name{ 
				(const char*)XStr().copy(reinterpret_cast<const char*>(name), bytes), (const unsigned int)bytes };
			workerName = worker_name;
			workerPtr.swap(ptr);
			pollDataThread = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&WorkerMode::pollDataFromWorkerThread, this));
			keepaliveThread = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&WorkerMode::keepAliveWorkerThread, this));
		}
	}

	return ret;
}

int WorkerMode::stop()
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

int WorkerMode::send(const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
	int ret{ workerPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		ret = workerPtr->send(data, bytes);
	}

	return ret;
}

void WorkerMode::pollDataFromWorkerThread()
{
	while (workerPtr && !stopped)
	{
		workerPtr->poll();
	}
}

void WorkerMode::keepAliveWorkerThread()
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
			url.setHost(workerName);
			url.addParameter("timestamp", (boost::format("%ld") % currentTickCount).str());
			url.addParameter("sequence", (boost::format("%d") % sequence++).str());
			const std::string data{url.encode()};

			if(!data.empty())
			{
				send(data.c_str(), data.length());
			}

			lastTickCout = currentTickCount;
		}

		xt.sleep(500);
	}
}
