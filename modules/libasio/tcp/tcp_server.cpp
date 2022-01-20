#include "boost/make_shared.hpp"
#include "error_code.h"
#include "tcp_server.h"
using namespace module::network::asio;

TcpServer::TcpServer()
{
}

TcpServer::~TcpServer()
{
	stop();
}

int TcpServer::start(const unsigned short port /*= 0*/)
{
	int ret{0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ret = service.start();

		if (Error_Code_Success == ret)
		{
			boost::shared_ptr<Acceptor> acceptor{
				boost::make_shared<Acceptor>(
					*service.ctx(),
					[&](boost::asio::ip::tcp::socket* so, const int e)
					{
						fetchAcceptedEventNotification(so, e);

						//持续监听
						if (!e && acceptorPtr)
						{
							acceptorPtr->listen(*service.ctx());
						}
					},
					port)};

			if (acceptor)
			{
				for (int i = 0; i != service.size(); ++i)
				{
					acceptor->listen(*service.ctx());
				}

				acceptorPtr.swap(acceptor);
			}
		}
	}

	return ret;
}

int TcpServer::stop()
{
	return service.stop();
}
