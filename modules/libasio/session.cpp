#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "session.h"
using namespace module::network::asio;

Session::Session(boost::asio::io_context& ctx) 
	: buffer{ nullptr }, bufBytes{0}, so{ctx}
{}

Session::~Session()
{
	destroy();
}

int Session::createNew(
	SentDataEventCallback sent, 
	ReceivedDataEventCallback recved, 
	const unsigned int bytes/* = 1048576*/)
{
	boost::checked_array_delete(reinterpret_cast<char*>(buffer));
	buffer = new(std::nothrow) unsigned char[bytes];
	int ret{buffer ? Error_Code_Success : Error_Code_Bad_New_Memory};

	if(Error_Code_Success == ret)
	{
		bufBytes = bytes;
		sentDataEventCBFunc = sent;
		receivedDataEventCBFunc = recved;
	}

	return ret;
}

int Session::destroy()
{
	so.close();
	boost::checked_array_delete(reinterpret_cast<char*>(buffer));
	return !so.is_open() && !buffer ? Error_Code_Success : Error_Code_Operate_Failure;
}

int Session::send(
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
			so.async_write_some(
				boost::asio::buffer((const char*)data + pos, transferred),
				[&](boost::system::error_code e, std::size_t bytes_transferred)
				{
					if (sentDataEventCBFunc)
					{
						sentDataEventCBFunc(e.value(), static_cast<int>(bytes_transferred));
					}
				});
			pos += transferred;
		}
	}

	return ret;
}

int Session::receive()
{
	int ret{ buffer && 0 < bufBytes ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		so.async_read_some(
			boost::asio::buffer(buffer, bufBytes),
			[&](boost::system::error_code e, std::size_t bytes_transferred)
			{
				if (receivedDataEventCBFunc)
				{
					receivedDataEventCBFunc(e.value(), buffer, static_cast<int>(bytes_transferred));
				}
			});
	}

	return ret;
}
