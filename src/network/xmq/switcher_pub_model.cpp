#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "network/xmq/switcher_pub_model.h"
using namespace framework::network::xmq;

SwitcherPubModel::SwitcherPubModel() : thread{nullptr}, stopped{ false }
{}

SwitcherPubModel::~SwitcherPubModel()
{
	stop();
}

int SwitcherPubModel::start(
	const unsigned short switcherPort /* = 0 */, 
	const unsigned short publisherPort /* = 0 */, 
	const int hwm /* = 10 */)
{
	if(switcherPtr || publisherPtr)
	{
		return Error_Code_Object_Existed;
	}

	int ret{0 < switcherPort && 0 < publisherPort ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		SwitcherPtr switcher{
			boost::make_shared<Switcher>(
				boost::bind(&SwitcherPubModel::afterSwitcherPolledDataHandler, this, _1, _2))};
		PublisherPtr publisher{boost::make_shared<Publisher>()};

		if(switcher && Error_Code_Success == switcher->bind(switcherPort) && 
			publisher && Error_Code_Success == publisher->bind(publisherPort, hwm))
		{
			switcherPtr.swap(switcher);
			publisherPtr.swap(publisher);
			thread = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&SwitcherPubModel::pollDataFromSwitcherThread, this));
		}
	}

	return ret;
}

int SwitcherPubModel::stop()
{
	int ret{switcherPtr && publisherPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		stopped = true;
		Thread().join(thread);
		ThreadPool().get_mutable_instance().destroy(thread);
		switcherPtr.reset();
		publisherPtr.reset();
	}

	return ret;
}

int SwitcherPubModel::send(
	const std::string uid, 
	const std::string data)
{
	int ret{ switcherPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		ret = switcherPtr->send(uid, data);
	}

	return ret;
}

int SwitcherPubModel::send(const std::string data)
{
	int ret{ publisherPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		ret = publisherPtr->send(data);
	}

	return ret;
}

void SwitcherPubModel::pollDataFromSwitcherThread()
{
	while (switcherPtr && !stopped)
	{
		switcherPtr->poll();
	}
}
