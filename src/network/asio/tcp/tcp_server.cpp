#include "boost/make_shared.hpp"
#include "error_code.h"
#include "network/asio/tcp/tcp_server.h"
using namespace framework::network::asio;

TcpServer::TcpServer() : Server()
{}

TcpServer::~TcpServer()
{
	stop();
}

int TcpServer::start(const unsigned short port /*= 0*/)
{
	stop();
	IoServicePtr service{boost::make_shared<IoService>()};

	if (service && Error_Code_Success == service->createNew())
	{
		IoAcceptorPtr acceptor{
			boost::make_shared<IoAcceptor>(
				service->getIdle(), 
				[&](IoSessionPtr sessionPtr, const int e)
				{
					fetchAcceptedEventNotification(sessionPtr, e);
					
					//持续监听
					if (!e && ioAcceptorPtr)
					{
						ioAcceptorPtr->listen(ioServicePtr->getIdle());
					}
				}, 
				port)};
		
		if (acceptor)
		{
			for(int i = 0; i != service->getCount(); ++i)
			{
				acceptor->listen(service->getIdle());
			}

			ioServicePtr.swap(service);
			ioAcceptorPtr.swap(acceptor);
		}
	}
	
	return ioServicePtr && ioAcceptorPtr ? Error_Code_Success : Error_Code_Bad_New_Object;
}

int TcpServer::stop()
{
	ioServicePtr.reset();
	ioAcceptorPtr.reset();
	return Error_Code_Success;
}
