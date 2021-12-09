#include "boost/make_shared.hpp"
#include "error_code.h"
#include "session.h"
#include "connector.h"
using namespace module::network::asio;

Connector::Connector(RemoteConnectedEventCallback callback)
	: remoteConnectedCBFunc{ callback }
{}

Connector::~Connector()
{}

int Connector::connect(
	boost::asio::io_context& io, 
	const char* ip /* = nullptr */, 
	const unsigned short port /* = 0 */)
{
	int ret{ip && 0 < port ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		SessionPtr ptr{boost::make_shared<Session>(io)};

		if (ptr)
		{
			auto self{
				boost::enable_shared_from_this<Connector>::shared_from_this()};
			ptr->sock().async_connect(
				boost::asio::ip::tcp::endpoint(
					boost::asio::ip::address::from_string(ip), port),
				[&](boost::system::error_code e)
				{
					if (remoteConnectedCBFunc)
					{
						remoteConnectedCBFunc(ptr, e.value());
					}
				});
		}
		else
		{
			ret = Error_Code_Bad_New_Object;
		}
	}

	return ret;
}
