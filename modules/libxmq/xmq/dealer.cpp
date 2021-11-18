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

void* Dealer::connect(
	const std::string uid,
	const std::string ip, 
	const unsigned short port /* = 0 */,
	void* c /* = nullptr */)
{
	void* s{nullptr};

	if (c && !uid.empty() && !ip.empty() && 0 < port)
	{
		s = zmq_socket(c, ZMQ_DEALER);

		if (s)
		{
			int keepalive{ 1 }, idle{30};
			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE, &keepalive, sizeof(int));
			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE_IDLE, &idle, sizeof(int));
			zmq_setsockopt(s, ZMQ_IDENTITY, uid.c_str(), uid.length());

			if (zmq_connect(s, (boost::format("tcp://%s:%d") % ip % port).str().c_str()))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}

	return s;
}

int Dealer::shutdown(void* s /* = nullptr */)
{
	int ret{s ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		ret = (!zmq_close(s) ? Error_Code_Success : Error_Code_Operate_Failure);
	}
	
	return ret;
}
