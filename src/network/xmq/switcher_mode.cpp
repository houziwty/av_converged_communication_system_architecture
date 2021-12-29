#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "network/xmq/switcher_mode.h"
using namespace framework::network::xmq;

SwitcherMode::SwitcherMode() : thread{nullptr}, stopped{ false }
{}

SwitcherMode::~SwitcherMode()
{
	stop();
}

int SwitcherMode::start(const unsigned short port /* = 0 */)
{
	if(switcherPtr)
	{
		return Error_Code_Object_Existed;
	}

	int ret{0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		SwitcherPtr switcher{
			boost::make_shared<Switcher>(
				boost::bind(&SwitcherMode::afterSwitcherPolledDataHandler, this, _1, _2))};

		if(switcher && Error_Code_Success == switcher->bind(port))
		{
			switcherPtr.swap(switcher);
			thread = ThreadPool().get_mutable_instance().createNew(
				boost::bind(&SwitcherMode::pollDataFromSwitcherThread, this));
		}
	}

	return ret;
}

int SwitcherMode::stop()
{
	int ret{switcherPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		stopped = true;
		Thread().join(thread);
		ThreadPool().get_mutable_instance().destroy(thread);
		switcherPtr.reset();
	}

	return ret;
}

int SwitcherMode::send(
	const void* uid/* = nullptr*/, 
	const int uid_bytes/* = 0*/, 
	const void* data/* = nullptr*/, 
	const int data_bytes/* = 0*/)
{
	int ret{ switcherPtr && !stopped ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		ret = switcherPtr->send(uid, data);
	}

	return ret;
}

void SwitcherMode::pollDataFromSwitcherThread()
{
	while (switcherPtr && !stopped)
	{
		switcherPtr->poll();
	}
}
