#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "error_code.h"
#include "service.h"
#include "acceptor.h"
using namespace module::network::asio;

Acceptor::Acceptor(
	Service& ios, 
	AcceptedRemoteConnectEventCallback callback,
	const uint16_t port /* = 0 */)
	: service{ios}, acceptedRemoteConnectEventCallback{callback}, 
	acceptor{*service.ctx(), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)}
{
	acceptor.listen();
}

Acceptor::~Acceptor()
{}

int Acceptor::listen()
{
	int ret{Error_Code_Bad_New_Socket};
	boost::asio::ip::tcp::socket* so{
		new(std::nothrow) boost::asio::ip::tcp::socket{*service.ctx()}};

	if(so)
	{
		acceptor.async_accept(
			*so, 
			boost::bind(
				&Acceptor::afterAcceptedRemoteCallback, 
				boost::enable_shared_from_this<Acceptor>::shared_from_this(), 
				boost::asio::placeholders::error, 
				so));
		ret = Error_Code_Success;
	}

	return ret;
}

void Acceptor::afterAcceptedRemoteCallback(
	boost::system::error_code e, 
	boost::asio::ip::tcp::socket* s/* = nullptr*/)
{
	if (acceptedRemoteConnectEventCallback)
	{
		acceptedRemoteConnectEventCallback(s, e.value());
	}
	
	if (e.value())
	{
		s->close();
		boost::checked_delete(s);
	}
}
