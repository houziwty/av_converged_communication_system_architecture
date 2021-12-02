#include "io_listen.h"
using namespace module::network::asio;

IoListen::IoListen(
	boost::asio::io_context& io,
	const unsigned short port /* = 10000 */)
	: acceptor{io, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)}
{
	acceptor.listen();
}

IoListen::~IoListen()
{}

boost::asio::ip::tcp::acceptor& IoListen::getListener()
{
	return acceptor;
}
