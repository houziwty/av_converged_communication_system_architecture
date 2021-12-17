#include "boost/make_shared.hpp"
#include "error_code.h"
#include "network/xmq/pub_model.h"
using namespace framework::network::xmq;

PubModel::PubModel()
{}

PubModel::~PubModel()
{}

int PubModel::start(
	const unsigned short port /* = 0 */, 
	const int hwm /* = 10 */)
{
	if(publisherPtr)
	{
		return Error_Code_Object_Existed;
	}

	int ret{0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		PublisherPtr publisher{boost::make_shared<Publisher>()};

		if(publisher && Error_Code_Success == publisher->bind(port, hwm))
		{
			publisherPtr.swap(publisher);
		}
	}

	return ret;
}

int PubModel::stop()
{
	publisherPtr.reset();
	return Error_Code_Success;
}

int PubModel::send(const std::string data)
{
	int ret{ publisherPtr ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		ret = publisherPtr->send(data);
	}

	return ret;
}
