#include "boost/asio.hpp"
#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "udp_session.h"
using namespace module::network::asio;

UdpSession::UdpSession(
	void* s/* = nullptr*/, 
	const uint32_t id/* = 0*/) 
    : Session(s, id)
{}

UdpSession::~UdpSession()
{}

int UdpSession::destroy()
{
    int ret{so ? Error_Code_Success : Error_Code_Operate_Failure};

    if (Error_Code_Success == ret)
    {
        boost::asio::ip::udp::socket* s{
            reinterpret_cast<boost::asio::ip::udp::socket*>(so)};
        s->close();
        boost::checked_delete(s);
    }
    
    return Error_Code_Success == ret ? Session::destroy() : ret;
}

int UdpSession::send(
	const void* data /* = nullptr */, 
	const uint64_t bytes /* = 0 */, 
	const char* ip /* = nullptr */, 
	const uint16_t port /* = 0 */)
{
	int ret{ so && data && 0 < bytes && ip && 0 < port ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
        boost::asio::ip::udp::socket* s{
            reinterpret_cast<boost::asio::ip::udp::socket*>(so)};
		boost::asio::ip::udp::endpoint dest(
			boost::asio::ip::address::from_string(ip), port);

		s->async_send_to(
			boost::asio::buffer((const char*)data, bytes), 
			dest, 
			[&](boost::system::error_code e, std::size_t bytes_transferred)
			{
				if (sentDataEventCallback)
				{
					sentDataEventCallback(sid, bytes_transferred, e.value());
				}
			});
	}

	return ret;
}

int UdpSession::receive()
{
	int ret{ so && buffer && 0 < bufBytes ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
        boost::asio::ip::udp::socket* s{
            reinterpret_cast<boost::asio::ip::udp::socket*>(so)};
		boost::asio::ip::udp::endpoint from;
        
		s->async_receive_from(
			boost::asio::buffer(buffer, bufBytes), 
			from, 
			[&](boost::system::error_code e, std::size_t bytes_transferred)
			{
				if (receivedDataEventCallback)
				{
					receivedDataEventCallback(sid, buffer, bytes_transferred, e.value());
				}

                if (!e)
                {
                    receive();
                }
			});
	}

	return ret;
}
