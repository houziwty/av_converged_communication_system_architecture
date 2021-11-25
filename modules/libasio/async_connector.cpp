#include "error_code.h"
#include "async_connector.h"
using namespace module::network::asio;

AsyncConnector::AsyncConnector(AsyncConnectEventCallback callback /* = nullptr */)
	: asyncConnectEventCallback{ callback }
{}

AsyncConnector::~AsyncConnector()
{}

int AsyncConnector::connect(
	boost::asio::io_context& io, 
	const char* ip /* = nullptr */, 
	const unsigned short port /* = 0 */)
{
	int ret{ip && 0 < port ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		auto self{
			boost::enable_shared_from_this<AsyncConnector>::shared_from_this()};
		boost::asio::ip::tcp::socket s{io};
		s.async_connect(
			boost::asio::ip::tcp::endpoint(
				boost::asio::ip::address::from_string(ip), port),
			[&](boost::system::error_code e/*, boost::asio::ip::tcp::endpoint*/)
			{
				if (asyncConnectEventCallback)
				{
					asyncConnectEventCallback(s, e.value());
				}
			});
	}

	return ret;
}
