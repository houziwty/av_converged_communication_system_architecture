#include "boost/make_shared.hpp"
#include "error_code.h"
#include "libasio/acceptor.h"
#include "tcp_server.h"
using namespace module::network::asio;

TcpServer::TcpServer()
{}

TcpServer::~TcpServer()
{
	stop();
}

int TcpServer::start(const unsigned short port /*= 0*/)
{
	int ret{0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
//		stop();
		ret = service.start();

		if (Error_Code_Success == ret)
		{
			boost::shared_ptr<Acceptor> acceptor{
				boost::make_shared<Acceptor>(
					*service.ctx(), 
					[&](SessionPtr session, const int e)
					{
						fetchAcceptedEventNotification(session, e);
						
						//持续监听
						if (!e && acceptor)
						{
							acceptor->listen(*service.ctx());
						}
					}, 
					port)};
			
			if (acceptor)
			{
				for(int i = 0; i != service.size(); ++i)
				{
					acceptor->listen(*service.ctx());
				}
			}
		}
	}
	
	return ret;
}

int TcpServer::stop()
{
	return service.stop();
}
