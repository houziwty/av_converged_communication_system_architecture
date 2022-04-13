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

Router::Router() : Sock()
{}

Router::~Router()
{}

void* Router::bind(
	void* ctx /* = nullptr */, 
	const uint16_t port /* = 0 */, 
	const uint64_t hwm /* = 0 */)
{
	void* s{nullptr};

	if (ctx && 0 < port)
	{
		s = zmq_socket(ctx, ZMQ_ROUTER);

		if(s)
		{
			if (0 < hwm)
			{
				zmq_setsockopt(s, ZMQ_RCVHWM, &hwm, sizeof(const uint64_t));
				zmq_setsockopt(s, ZMQ_SNDHWM, &hwm, sizeof(const uint64_t));
			}
			
			if(zmq_bind(s, (boost::format("tcp://*:%d") % port).str().c_str()))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}

	return s;
}
