#include "error_code.h"
#include "io_listen.h"
#include "async_acceptor.h"
using namespace module::network::asio;

AsyncAcceptor::AsyncAcceptor(
	boost::asio::io_context& io, 
	AsyncAcceptEventCallback callback /* = nullptr */)
	: so{io}, asyncAcceptEventCallback{ callback }
{}

AsyncAcceptor::~AsyncAcceptor()
{}

int AsyncAcceptor::accept(IoListenPtr listenPtr)
{
	int ret{listenPtr ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		auto self{ 
			boost::enable_shared_from_this<AsyncAcceptor>::shared_from_this() };

		listenPtr->getListener().async_accept(
			so,
			[this, self](boost::system::error_code e)
			{
				if (asyncAcceptEventCallback)
				{
					asyncAcceptEventCallback(so, e.value());
				}
			});
	}

	return ret;
}