#include "boost/asio.hpp"
#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "tcp_session.h"
using namespace module::network::asio;

TcpSession::TcpSession(
    void* s/* = nullptr*/, 
	const uint32_t id/* = 0*/) 
    : Session(s, id)
{}

TcpSession::~TcpSession()
{}

int TcpSession::destroy()
{
    int ret{so ? Error_Code_Success : Error_Code_Operate_Failure};

    if (Error_Code_Success == ret)
    {
        boost::asio::ip::tcp::socket* s{
            reinterpret_cast<boost::asio::ip::tcp::socket*>(so)};
        s->close();
        boost::checked_delete(s);
    }
    
    return Error_Code_Success == ret ? Session::destroy() : ret;
}

int TcpSession::send(
	const void* data /* = nullptr */, 
	const uint64_t bytes /* = 0 */, 
    const char* /* ip = nullptr*/, 
    const uint16_t/* port = 0*/)
{
	int ret{ !state() && so && data && 0 < bytes ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		int pos{ 0 }, transferred{ 0 };
        boost::asio::ip::tcp::socket* s{
            reinterpret_cast<boost::asio::ip::tcp::socket*>(so)};

		while (pos < bytes)
		{
			transferred = bytes - pos;
			transferred = (transferred > 1048576 ? 1048576 : transferred);
			s->async_write_some(
				boost::asio::buffer((const char*)data + pos, transferred), 
				boost::bind(
					&TcpSession::afterAsyncWriteSomeCallback, 
					boost::enable_shared_from_this<TcpSession>::shared_from_this(), 
					_1, _2)
				//It will be blocked in lambda expressions if tcp session has been shared from this.
// 				[&](boost::system::error_code e, std::size_t bytes_transferred)
// 				{
// 					if (sentDataEventCallback)
// 					{
// 						sentDataEventCallback(sid, bytes_transferred, e.value());
// 					}
// 				}
			);
			pos += transferred;
		}
	}

	return ret;
}

int TcpSession::receive()
{
	int ret{ so && buffer && 0 < bufBytes ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
        boost::asio::ip::tcp::socket* s{
            reinterpret_cast<boost::asio::ip::tcp::socket*>(so)};
        
		s->async_read_some(
			boost::asio::buffer(buffer, bufBytes), 
			boost::bind(
				&TcpSession::afterAsyncReadSomeCallback,
				boost::enable_shared_from_this<TcpSession>::shared_from_this(),
				_1, _2)
			//It will be blocked in lambda expressions if tcp session has been shared from this.
// 			[&, self](boost::system::error_code e, std::size_t bytes_transferred)
// 			{
// 				const int32_t ec{ e.value() };
// 
// 				if (receivedDataEventCallback)
// 				{
// 					receivedDataEventCallback(sid, buffer, bytes_transferred, ec);
// 				}
// 
//                 if (!ec)
//                 {
//                     receive();
//                 }
// 			}
		);
	}

	return ret;
}
