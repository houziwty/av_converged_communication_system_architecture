#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "boost/format.hpp"
#include "libxmq/worker.h"
using namespace module::network::xmq;
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "utils/time/xtime.h"
using namespace framework::utils::time;
#include "network/xmq/worker_deal.h"
using namespace framework::network::xmq;

WorkerDeal::WorkerDeal() 
	: worker{nullptr}, thread{nullptr}, stopped{ false }
{}

WorkerDeal::~WorkerDeal()
{
	stop();
}

int WorkerDeal::start(
	const std::string uid, 
	const std::string ip, 
	const unsigned short port /*= 0*/)
{
	int ret{ !worker ? Error_Code_Success : Error_Code_Object_Existed};

	if (Error_Code_Success == ret)
	{
		ret = (!ip.empty() && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Worker* w{
				new(std::nothrow) Worker(
					boost::bind(&WorkerDeal::afterWorkerPolledDataHandler, this, _1))};

			if(w && Error_Code_Success == w->connect(uid, ip, port))
			{
				thread = ThreadPool().get_mutable_instance().createNew(
					boost::bind(&WorkerDeal::pollDataFromWorkerThread, this));
				worker = w;
			}
			else
			{
				boost::checked_delete(worker);
				ret = Error_Code_Bad_New_Object;
			}
		}
	}

	return ret;
}

int WorkerDeal::stop()
{
	stopped = true;
	Thread().join(thread);
	ThreadPool().get_mutable_instance().destroy(thread);
	boost::checked_delete(worker);

	return Error_Code_Success;
}

int WorkerDeal::send(const std::string data)
{
	int ret{ worker ? Error_Code_Success : Error_Code_Object_Existed };

	if (Error_Code_Success == ret)
	{
		Worker* w{reinterpret_cast<Worker*>(worker)};
		ret = w->send(data);
	}

	return ret;
}

int WorkerDeal::keepalive(
	const std::string name, 
	const int interval/* = 30*/)
{
	int ret{!name.empty() && 0 < interval ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		ThreadPool().get_mutable_instance().createNew(
			boost::bind(&WorkerDeal::keepAliveWorkerThread, this, name, interval));
	}

	return ret;
}

void WorkerDeal::pollDataFromWorkerThread()
{
	while (!stopped)
	{
		Worker* w{reinterpret_cast<Worker*>(worker)};
		w->poll();
	}
}

void WorkerDeal::keepAliveWorkerThread(const std::string name, const int interval/* = 30*/)
{
	unsigned long long lastTickCout{0};
	XTime xt;

	while (!stopped)
	{
		unsigned long long currentTickCount{xt.tickcount()};

		if(currentTickCount - lastTickCout > interval * 1000)
		{
			//Register
			//register://name?tick=currentTickCount
			send((boost::format("register://%s?tick=ld") % name % interval).str());
		}

		xt.sleep(500);
	}
}
