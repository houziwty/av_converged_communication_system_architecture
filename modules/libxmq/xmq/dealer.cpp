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

socket_t Dealer::connect(
	const void* uid/* = nullptr*/, 
	const int uid_bytes/* = 0*/, 
	const char* ip/* = nullptr*/, 
	const unsigned short port /* = 0 */,
	ctx_t c /* = nullptr */)
{
	socket_t s{nullptr};

	if (c && uid && ip && 0 < port)
	{
		s = zmq_socket(c, ZMQ_DEALER);

		if (s)
		{
			int keepalive{ 1 }, idle{30};
			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE, &keepalive, sizeof(int));
			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE_IDLE, &idle, sizeof(int));
			zmq_setsockopt(s, ZMQ_IDENTITY, uid, uid_bytes);

			if (zmq_connect(s, (boost::format("tcp://%s:%d") % ip % port).str().c_str()))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}

	return s;
}

int Dealer::shutdown(socket_t s /* = nullptr */)
{
	int ret{s ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		ret = (!zmq_close(s) ? Error_Code_Success : Error_Code_Operate_Failure);
	}
	
	return ret;
}
