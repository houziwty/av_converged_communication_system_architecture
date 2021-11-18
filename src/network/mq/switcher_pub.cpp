#include "boost/checked_delete.hpp"
#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "libxmq/switcher.h"
#include "libxmq/publisher.h"
using namespace module::network::xmq;
#include "error_code.h"
#include "utils/thread/thread.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;
#include "network/mq/switcher_pub.h"
using namespace framework::network::mq;

SwitcherPub::SwitcherPub() 
	: switcher{nullptr}, publisher{nullptr}, thread{nullptr}, stopped{ false }
{}

SwitcherPub::~SwitcherPub()
{
	stop();
}

int SwitcherPub::start(
	const unsigned short switcherPort /* = 0 */, 
	const unsigned short publisherPort /* = 0 */, 
	const int hwm /* = 10 */)
{
	int ret{ !switcher && !publisher ? Error_Code_Success : Error_Code_Object_Existed};

	if (Error_Code_Success == ret)
	{
		ret = (0 < switcherPort && 0 < publisherPort ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Switcher* ps{
				new(std::nothrow) Switcher(
					boost::bind(&SwitcherPub::afterSwitcherPolledDataHandler, this, _1, _2))};
			Publisher* pp{new(std::nothrow) Publisher()};

			if(ps && Error_Code_Success == ps->bind(switcherPort) && 
				pp && Error_Code_Success == pp->bind(publisherPort, hwm))
			{
				thread = ThreadPool().get_mutable_instance().createNew(
					boost::bind(&SwitcherPub::pollDataFromSwitcherThread, this));
				switcher = ps;
				publisher = pp;
			}
			else
			{
				boost::checked_delete(ps);
				boost::checked_delete(pp);
				ret = Error_Code_Bad_New_Object;
			}
		}
	}

	return ret;
}

int SwitcherPub::stop()
{
	stopped = true;
	Thread().join(thread);
	ThreadPool().get_mutable_instance().destroy(thread);
	boost::checked_delete(switcher);
	boost::checked_delete(publisher);
	switcher = nullptr;
	publisher = nullptr;

	return Error_Code_Success;
}

int SwitcherPub::send(
	const std::string uid, 
	const std::string data)
{
	int ret{ switcher ? Error_Code_Success : Error_Code_Object_Existed };

	if (Error_Code_Success == ret)
	{
		Switcher* ps{reinterpret_cast<Switcher*>(switcher)};
		ret = ps->send(uid, data);
	}

	return ret;
}

int SwitcherPub::send(
	const std::string data)
{
	int ret{ publisher ? Error_Code_Success : Error_Code_Object_Existed };

	if (Error_Code_Success == ret)
	{
		Publisher* pp{reinterpret_cast<Publisher*>(publisher)};
		ret = pp->send(data);
	}

	return ret;
}

void SwitcherPub::pollDataFromSwitcherThread()
{
	while (!stopped)
	{
		Switcher* switcher{reinterpret_cast<Switcher*>(this->switcher)};
		switcher->poll();
	}
}
