#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "network/xmq/switcher_pub_mode.h"
using namespace framework::network::xmq;

SwitcherPubMode::SwitcherPubMode() : thread{nullptr}, stopped{ false }
{}

SwitcherPubMode::~SwitcherPubMode()
{
	stop();
}

int SwitcherPubMode::start(
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
				boost::bind(&SwitcherPubMode::afterSwitcherPolledDataHandler, this, _1, _2, _3, _4))};
		PublisherPtr publisher{boost::make_shared<Publisher>()};

		if(switcher && Error_Code_Success == switcher->bind(switcherPort) && 
			publisher && Error_Code_Success == publisher->bind(publisherPort, hwm))
		{
			switcherPtr.swap(switcher);
			publisherPtr.swap(publisher);
			thread = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&SwitcherPubMode::pollDataFromSwitcherThread, this));
		}
	}

	return ret;
}

int SwitcherPubMode::stop()
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

int SwitcherPubMode::switcherSend(
	const void* uid/* = nullptr*/, 
	const int uid_bytes/* = 0*/, 
	const void* data/* = nullptr*/, 
	const int data_bytes/* = 0*/)
{
	int ret{ switcherPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		ret = switcherPtr->send(uid, uid_bytes, data, data_bytes);
	}

	return ret;
}

int SwitcherPubMode::pubSend(const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
	int ret{ publisherPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		ret = publisherPtr->send(data, bytes);
	}

	return ret;
}

void SwitcherPubMode::pollDataFromSwitcherThread()
{
	while (switcherPtr && !stopped)
	{
		switcherPtr->poll();
	}
}
