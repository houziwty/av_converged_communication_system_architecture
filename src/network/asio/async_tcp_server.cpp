#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "boost/make_shared.hpp"
#include "libasio/async_acceptor.h"
using namespace module::network::asio;
#include "utils/hardware/cpu.h"
using namespace framework::utils::hardware;
#include "error_code.h"
#include "network/asio/async_tcp_session.h"
#include "network/asio/async_tcp_server.h"
using namespace framework::network::asio;

AsyncTcpServer::AsyncTcpServer()
{}

AsyncTcpServer::~AsyncTcpServer()
{
	destroy();
}

int AsyncTcpServer::createNew(const unsigned short port /*= 10000*/)
{
	if (ioServicePtr)
	{
		return Error_Code_Object_Existed;
	}
	
	int ret{0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		IoServicePtr iosPtr{boost::make_shared<IoService>()};

		if (iosPtr && Error_Code_Success == iosPtr->createNew())
		{
			IoListenPtr ptr{
				boost::make_shared<IoListen>(iosPtr->getIdle(), port)};
			if (ptr)
			{
				ioListenPtr.swap(ptr);
				//投递监听
				for(int i = 0; i != iosPtr->getCount(); ++i)
				{
					AsyncAcceptorPtr ptr{
						boost::make_shared<AsyncAcceptor>(
							iosPtr->getIdle(), 
							boost::bind(&AsyncTcpServer::asyncAcceptEventCallback, this, _1, _2))};

					if (ptr)
					{
						ptr->accept(ioListenPtr);	
					}
				}
			}

			ioServicePtr.swap(iosPtr);
		}
	}
	
	return ioServicePtr && ioListenPtr ? Error_Code_Success : Error_Code_Bad_New_Object;
}

int AsyncTcpServer::destroy()
{
	int ret{ioServicePtr ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		ioServicePtr.reset();
	}
	
	return ret;
}

void AsyncTcpServer::asyncAcceptEventCallback(boost::asio::ip::tcp::socket& s, const int e/* = 0*/)
{
	if (!e)
	{
		afterFetchAsyncAcceptEventNotification(s);

		//连续监听
		AsyncAcceptorPtr ptr{
			boost::make_shared<AsyncAcceptor>(
				ioServicePtr->getIdle(), 
				boost::bind(&AsyncTcpServer::asyncAcceptEventCallback, this, _1, _2))};

		if (ptr)
		{
			ptr->accept(ioListenPtr);	
		}
	}
}
