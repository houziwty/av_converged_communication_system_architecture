#include "boost/format.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "sub.h"
using namespace module::network::xmq;

Sub::Sub()
{}

Sub::~Sub()
{}

socket_t Sub::connect(
	const std::string ip, 
	const unsigned short port /* = 0 */,
	ctx_t c /* = nullptr */)
{
	socket_t s{nullptr};

	if (c && !ip.empty() && 0 < port)
	{
		s = zmq_socket(c, ZMQ_SUB);

		if (s)
		{
			zmq_setsockopt(s, ZMQ_SUBSCRIBE, "", 0);

			if (zmq_connect(s, (boost::format("tcp://%s:%d") % ip.c_str() % port).str().c_str()))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}

	return s;
}

int Sub::shutdown(socket_t s /* = nullptr */)
{
	int ret{s ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		ret = (!zmq_close(s) ? Error_Code_Success : Error_Code_Operate_Failure);
	}
	
	return ret;
}
