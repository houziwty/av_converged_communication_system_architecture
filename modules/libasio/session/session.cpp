#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "session.h"
using namespace module::network::asio;

Session::Session(
	void* s/* = nullptr*/, 
	const uint32_t id/* = 0*/)
	: buffer{ nullptr }, bufBytes{0}, so{s}, sid{id}, stopped{false}
{}

Session::~Session()
{
	destroy();
}

int Session::createNew(
	SentDataEventCallback sent, 
	ReceivedDataEventCallback recved, 
	const uint64_t bytes/* = 1048576*/)
{
	buffer = new(std::nothrow) unsigned char[0 < bytes ? bytes : 1048576];
	int ret{buffer ? Error_Code_Success : Error_Code_Bad_New_Memory};

	if(Error_Code_Success == ret)
	{
		bufBytes = (0 < bytes ? bytes : 1048576);
		sentDataEventCallback = sent;
		receivedDataEventCallback = recved;
	}

	return ret;
}

int Session::destroy()
{
	int ret{ buffer && 0 < sid ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		boost::checked_array_delete(reinterpret_cast<char*>(buffer));
		buffer = nullptr;
		bufBytes = 0;
	}
	
	return ret;
}

void Session::afterAsyncWriteSomeCallback(
	const boost::system::error_code e, 
	const std::size_t bytes_transferred)
{
	if (e)
	{
		stopped = true;
	}

	if (sentDataEventCallback)
	{
		sentDataEventCallback(sid, bytes_transferred, e.value());
	}
}

void Session::afterAsyncReadSomeCallback(
	const boost::system::error_code e, 
	const std::size_t bytes_transferred)
{
	if (e)
	{
		stopped = true;
	}

	if (receivedDataEventCallback)
	{
		receivedDataEventCallback(sid, buffer, bytes_transferred, e.value());
	}

	if (!e)
	{
		receive();
	}
}
