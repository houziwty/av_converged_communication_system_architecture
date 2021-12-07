#include "error_code.h"
#include "network/asio/session.h"
using namespace framework::network::asio;

Session::Session(IoSessionPtr sess) : ioSessionPtr{sess}
{}

Session::~Session()
{
	destroy();
}

int Session::createNew(const unsigned int bytes /*= 1048576*/)
{
	int ret{ioSessionPtr ? Error_Code_Success : Error_Code_Bad_New_Object};

	if (Error_Code_Success != ioSessionPtr->createNew(
			[&](const int e, const unsigned int bytes)
			{
				fetchSentDataEventNotification(e, bytes);
			}, 
			[&](const int e, const void* data, const unsigned int bytes)
			{
				fetchReceivedDataEventNotification(e, data, bytes);
				
				//持续接收
				if(!e)
				{
					ioSessionPtr->receive();
				}
			}, 
			0 < bytes ? bytes : 1048576))
	{
		ret = Error_Code_Operate_Failure;
	}
	
	return ret;
}

int Session::destroy()
{
	int ret{ioSessionPtr ? Error_Code_Success : Error_Code_Bad_New_Object};

	if (Error_Code_Success == ret)
	{
		ioSessionPtr.reset();
	}
	
	return ioSessionPtr ? Error_Code_Operate_Failure : Error_Code_Success;
}

int Session::send(const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
	int ret{ioSessionPtr ? Error_Code_Success : Error_Code_Bad_New_Object};

	if (Error_Code_Success == ret)
	{
		ret = (data && 0 < bytes ? ioSessionPtr->send(data, bytes) : Error_Code_Invalid_Param);
	}

	return ret;
}

int Session::receive()
{
	int ret{ioSessionPtr ? Error_Code_Success : Error_Code_Bad_New_Object};

	if (Error_Code_Success == ret)
	{
		ret = ioSessionPtr->receive();
	}

	return ret;
}
