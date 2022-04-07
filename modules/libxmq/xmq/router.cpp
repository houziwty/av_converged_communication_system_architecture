#include "boost/format.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "router.h"
using namespace module::network::xmq;

Router::Router()
{}

Router::~Router()
{}

xsocket Router::bind(
	xctx c /* = nullptr */, 
	const unsigned short port /* = 0 */)
{
	xsocket s{nullptr};

	if (c && 0 < port)
	{
		s = zmq_socket(c, ZMQ_ROUTER);
		int queue_size{30}, buf_size{10 * 1024 * 1024};
		//zmq_setsockopt(s, ZMQ_SNDHWM, &queue_size,sizeof(int));
		//zmq_setsockopt(s, ZMQ_RCVHWM, &queue_size, sizeof(int));
		//zmq_setsockopt(s, ZMQ_RCVBUF, &buf_size, sizeof(int));
		//zmq_setsockopt(s, ZMQ_SNDBUF, &buf_size, sizeof(int));

		if(s)
		{
			if(zmq_bind(s, (boost::format("tcp://*:%d") % port).str().c_str()))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}

	return s;
}

int Router::shutdown(xsocket s /* = nullptr */)
{
	int ret{s ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		ret = (!zmq_close(s) ? Error_Code_Success : Error_Code_Operate_Failure);
	}
	
	return ret;
}
