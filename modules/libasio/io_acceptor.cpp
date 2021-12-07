#include "boost/make_shared.hpp"
#include "error_code.h"
#include "io_acceptor.h"
using namespace module::network::asio;

IoAcceptor::IoAcceptor(
	boost::asio::io_context& ctx, 
	RemoteConnectedEventCallback callback, 
	const unsigned short port /* = 0 */)
	: acceptor{ctx, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)}, 
	remoteConnectedCBFunc{callback}
{
	acceptor.listen();
}

IoAcceptor::~IoAcceptor()
{}

int IoAcceptor::listen(boost::asio::io_context& ctx)
{
	IoSessionPtr ptr{boost::make_shared<IoSession>(ctx)};

	if(ptr)
	{
		acceptor.async_accept(
			ptr->sock(), 
			[&, ptr](boost::system::error_code e)
			{
				if (remoteConnectedCBFunc)
				{
					remoteConnectedCBFunc(ptr, e.value());
				}
			});
	}

	return ptr ? Error_Code_Success : Error_Code_Bad_New_Socket;
}
