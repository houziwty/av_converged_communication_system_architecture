#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "network/xmq/sub_model.h"
using namespace framework::network::xmq;

SubModel::SubModel() : thread{nullptr}, stopped{ false }
{}

SubModel::~SubModel()
{
	stop();
}

int SubModel::start(
	const std::string ip, 
	const unsigned short port /* = 0 */)
{
	if(subscriberPtr)
	{
		return Error_Code_Object_Existed;
	}

	int ret{!ip.empty() && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		SubscriberPtr subscriber{
			boost::make_shared<Subscriber>(
				boost::bind(&SubModel::afterWorkerPolledDataHandler, this, _1))};

		if(subscriber && Error_Code_Success == subscriber->connect(ip, port))
		{
			subscriberPtr.swap(subscriber);
			thread = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&SubModel::pollDataFromWorkerThread, this));
		}
	}

	return ret;
}

int SubModel::stop()
{
	int ret{subscriberPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		stopped = true;
		Thread().join(thread);
		ThreadPool().get_mutable_instance().destroy(thread);
		subscriberPtr.reset();
	}

	return ret;
}

void SubModel::pollDataFromWorkerThread()
{
	while (subscriberPtr && !stopped)
	{
		subscriberPtr->poll();
	}
}
