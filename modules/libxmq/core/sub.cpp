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

Sub::Sub() : Sock()
{}

Sub::~Sub()
{}

void* Sub::connect(
	void* ctx /* = nullptr */, 
	const char* ip/* = nullptr*/, 
	const uint16_t port /* = 0 */, 
	const uint64_t reconivl /* = 0 */)
{
	void* s{nullptr};

	if (ctx && ip && 0 < port)
	{
		s = zmq_socket(ctx, ZMQ_SUB);

		if (s)
		{
			zmq_setsockopt(s, ZMQ_SUBSCRIBE, "", 0);

			if (0 < reconivl)
			{
				zmq_setsockopt(s, ZMQ_RECONNECT_IVL, &reconivl, sizeof(const uint64_t));
			}

			if (zmq_connect(s, (boost::format("tcp://%s:%d") % ip % port).str().c_str()))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}

	return s;
}
