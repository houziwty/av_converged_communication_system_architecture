#include "boost/make_shared.hpp"
#include "error_code.h"
#include "connector.h"
using namespace module::network::asio;

Connector::Connector(ConnectedRemoteEventCallback callback)
	: connectedRemoteCBFunc{ callback }
{}

Connector::~Connector()
{}

int Connector::connect(
	boost::asio::io_context& ctx, 
	const char* ip /* = nullptr */, 
	const unsigned short port /* = 0 */)
{
	SessionPtr ptr{ boost::make_shared<Session>(ctx) };
	auto self{
		boost::enable_shared_from_this<Connector>::shared_from_this() };
	ptr->sock().async_connect(
		boost::asio::ip::tcp::endpoint(
			boost::asio::ip::address::from_string(ip), port),
		[&, self](boost::system::error_code e)
		{
			if (connectedRemoteCBFunc)
			{
				connectedRemoteCBFunc(ptr, e.value());
			}
		});

	return Error_Code_Success;
}
