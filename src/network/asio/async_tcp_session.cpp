#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "network/asio/async_tcp_session.h"
using namespace framework::network::asio;

AsyncTcpSession::AsyncTcpSession(boost::asio::ip::tcp::socket& s) : so{std::move(s)}
{}

AsyncTcpSession::~AsyncTcpSession()
{
	destroy();
}

int AsyncTcpSession::createNew(const int bytes /*= 1048576*/)
{
	if (ioSessionPtr)
	{
		return Error_Code_Object_Existed;
	}
	
	int ret{0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		IoSessionPtr ptr{boost::make_shared<IoSession>()};

		if (ptr && 
			Error_Code_Success == ptr->createNew(
				bytes, 
				boost::bind(&AsyncTcpSession::asyncSendEventCallback, this, _1, _2),
				boost::bind(&AsyncTcpSession::asyncReceiveEventCallback, this, _1, _2, _3)))
		{
			ioSessionPtr.swap(ptr);
		}
	}
	
	return ioSessionPtr ? Error_Code_Success : Error_Code_Bad_New_Object;
}

int AsyncTcpSession::destroy()
{
	int ret{ioSessionPtr ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		so.close();
		ioSessionPtr.reset();
	}
	
	return ret;
}

int AsyncTcpSession::send(const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
	int ret{ioSessionPtr ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		ret = (data && 0 < bytes ? ioSessionPtr->send(so, data, bytes) : Error_Code_Invalid_Param);
	}

	return ret;
}

int AsyncTcpSession::receive()
{
	int ret{ioSessionPtr ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		ret = ioSessionPtr->receive(so);
	}

	return ret;
}

void AsyncTcpSession::asyncSendEventCallback(const int e/* = 0*/, const int bytes/* = 0*/)
{
	afterFetchSendEventNotification(e, bytes);
}

void AsyncTcpSession::asyncReceiveEventCallback(const int e/* = 0*/, const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
	asyncFetchReceiveEventNotification(e, data, bytes);

	//持续接收
	if(!e && ioSessionPtr)
	{
		ioSessionPtr->receive(so);
	}
}
