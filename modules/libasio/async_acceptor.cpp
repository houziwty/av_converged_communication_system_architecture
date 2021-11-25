#include "error_code.h"
#include "async_acceptor.h"
using namespace module::network::asio;

AsyncAcceptor::AsyncAcceptor(
	boost::asio::io_context& io,
	const unsigned short port /* = 10000 */, 
	AsyncAcceptEventCallback callback /* = nullptr */)
	: acceptor{ io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port) }, 
	asyncAcceptEventCallback{ callback }
{}

AsyncAcceptor::~AsyncAcceptor()
{}

int AsyncAcceptor::listen()
{
	int ret{Error_Code_Success};
	auto self{ 
		boost::enable_shared_from_this<AsyncAcceptor>::shared_from_this() };

	acceptor.async_accept(
		[this, self](boost::system::error_code e, boost::asio::ip::tcp::socket s)
		{
			if (asyncAcceptEventCallback)
			{
				asyncAcceptEventCallback(s, e.value());
			}

			//持续监听
			if (!e)
			{
				listen();
			}
		});

	return ret;
}