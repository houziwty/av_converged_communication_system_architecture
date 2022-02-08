#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "service.h"
#include "connector.h"
using namespace module::network::asio;

Connector::Connector(
	Service& ios, 
	ConnectedRemoteEventCallback callback)
	: service{ios}, connectedRemoteEventCallback{ callback }
{}

Connector::~Connector()
{}

int Connector::connect(
	const char* ip /* = nullptr */, 
	const uint16_t port /* = 0 */)
{
	int ret{Error_Code_Bad_New_Socket};
	boost::asio::ip::tcp::socket* so{
		new(std::nothrow) boost::asio::ip::tcp::socket{*service.ctx()}};

	if (so)
	{
		so->async_connect(
			boost::asio::ip::tcp::endpoint(
				boost::asio::ip::address::from_string(ip), port), 
			boost::bind(
				&Connector::afterConntectedRemoteCallback, 
				boost::enable_shared_from_this<Connector>::shared_from_this(), 
				boost::asio::placeholders::error, 
				so));
		ret = Error_Code_Success;
	}

	return ret;
}

void Connector::afterConntectedRemoteCallback(
	boost::system::error_code e, 
	boost::asio::ip::tcp::socket* s/* = nullptr*/)
{
	if (connectedRemoteEventCallback)
	{
		connectedRemoteEventCallback(s, e.value());
	}
	
	if (e.value())
	{
		s->close();
		boost::checked_delete(s);
	}
}
