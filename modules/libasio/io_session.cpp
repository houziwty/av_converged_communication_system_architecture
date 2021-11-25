#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "io_session.h"
using namespace module::network::asio;

IoSession::IoSession() : buffer{ nullptr }, bufBytes{0}
{}

IoSession::~IoSession()
{
	destroy();
}

int IoSession::createNew(
	const int bytes/* = 1048576*/, 
	AsyncSendEventCallback scb/* = nullptr*/, 
	AsyncReceiveEventCallback rcb/* = nullptr*/)
{
	boost::checked_array_delete(buffer);
	buffer = new(std::nothrow) unsigned char[bytes];
	int ret{buffer ? Error_Code_Success : Error_Code_Bad_New_Memory};

	if(Error_Code_Success == ret)
	{
		bufBytes = bytes;
		asyncSendEventCallback = scb;
		asyncReceiveEventCallback = rcb;
	}

	return ret;
}

int IoSession::destroy()
{
	boost::checked_array_delete(buffer);
	return Error_Code_Success;
}

int IoSession::send(
	boost::asio::ip::tcp::socket& s, 
	const void* data /* = nullptr */, 
	const int bytes /* = 0 */)
{
	int ret{ data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		int pos{ 0 }, transferred{ 0 };

		while (pos < bytes)
		{
			transferred = bytes - pos;
			transferred = (transferred > 1048576 ? 1048576 : transferred);
			s.async_write_some(
				boost::asio::buffer(data + pos, transferred),
				[this](boost::system::error_code e, std::size_t bytes_transferred)
				{
					if (asyncSendEventCallback)
					{
						asyncSendEventCallback(e.value(), static_cast<int>(bytes_transferred));
					}
				});
			pos += transferred;
		}
	}

	return ret;
}

int IoSession::receive(boost::asio::ip::tcp::socket& s)
{
	int ret{ buffer && 0 < bufBytes ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		s.async_read_some(
			boost::asio::buffer(buffer, bufBytes),
			[this](boost::system::error_code e, std::size_t bytes_transferred)
			{
				if (asyncReceiveEventCallback)
				{
					asyncReceiveEventCallback(e.value(), buffer, static_cast<int>(bytes_transferred));
				}
			});
	}

	return ret;
}
