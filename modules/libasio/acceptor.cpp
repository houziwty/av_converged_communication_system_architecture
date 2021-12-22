#include "boost/make_shared.hpp"
#include "error_code.h"
#include "acceptor.h"
using namespace module::network::asio;

Acceptor::Acceptor(
	boost::asio::io_context& ctx, 
	AcceptedRemoteConnectEventCallback callback,
	const unsigned short port /* = 0 */)
	: acceptor{ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)}, 
	acceptedRemoteConnectCBFunc{callback}
{
	acceptor.listen();
}

Acceptor::~Acceptor()
{}

int Acceptor::listen(boost::asio::io_context& ctx)
{
	SessionPtr ptr{boost::make_shared<Session>(ctx)};
	auto self{
		boost::enable_shared_from_this<Acceptor>::shared_from_this() };
	acceptor.async_accept(
		ptr->sock(),
		[&, self](boost::system::error_code e)
		{
			if (acceptedRemoteConnectCBFunc)
			{
				acceptedRemoteConnectCBFunc(ptr, e.value());
			}
		});

	return Error_Code_Success;
}
