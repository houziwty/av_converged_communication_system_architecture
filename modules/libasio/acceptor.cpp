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
	int ret{Error_Code_Bad_New_Socket};
	boost::asio::ip::tcp::socket* so{
		new(std::nothrow) boost::asio::ip::tcp::socket{ctx}};

	if(so)
	{
		acceptor.async_accept(
			*so, 
			[&, so](boost::system::error_code e)
			{
				if (acceptedRemoteConnectCBFunc)
				{
					acceptedRemoteConnectCBFunc(so, e.value());
				}
			});

		ret = Error_Code_Success;
	}

	return ret;
}
