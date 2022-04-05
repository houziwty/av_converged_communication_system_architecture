#include "boost/format.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "dealer.h"
using namespace module::network::xmq;

Dealer::Dealer()
{}

Dealer::~Dealer()
{}

xsocket Dealer::connect(
	xctx c /* = nullptr */, 
	const char* uid/* = nullptr*/, 
	const char* ip/* = nullptr*/, 
	const uint16_t port /* = 0 */)
{
	xsocket s{nullptr};
	const std::string id{uid};

	if (c && uid && ip && 0 < port)
	{
		s = zmq_socket(c, ZMQ_DEALER);

		if (s)
		{
			int keepalive{ 1 }, idle{30};
			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE, &keepalive, sizeof(int));
			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE_IDLE, &idle, sizeof(int));
			zmq_setsockopt(s, ZMQ_IDENTITY, id.c_str(), id.length());

			if (zmq_connect(s, (boost::format("tcp://%s:%d") % ip % port).str().c_str()))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}

	return s;
}

xsocket Dealer::bind(
	xctx c /* = nullptr */, 
	const char* name/* = nullptr*/)
{
	xsocket s{nullptr};

	if (c)
	{
		s = zmq_socket(c, ZMQ_DEALER);

		if (s)
		{
			zmq_bind(s, name);
		}
	}
	
	return s;
}

xsocket Dealer::connect_task(
	xctx c /* = nullptr */, 
	const char* name/* = nullptr*/)
{
	xsocket s{nullptr};

	if (c)
	{
		s = zmq_socket(c, ZMQ_DEALER);

		if (s)
		{
			zmq_connect(s, name);
		}
	}
	
	return s;
}

int Dealer::shutdown(xsocket s /* = nullptr */)
{
	int ret{s ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		ret = (!zmq_close(s) ? Error_Code_Success : Error_Code_Operate_Failure);
	}
	
	return ret;
}
