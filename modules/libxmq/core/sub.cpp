#include "boost/format.hpp"
#include "zmq.h"
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

			//Set reconnection interval.
			//The ZMQ_RECONNECT_IVL option shall set the initial reconnection interval for the specified socket.
			//The reconnection interval is the period ØMQ shall wait between attempts to reconnect disconnected peers when using connection - oriented transports.
			//The value - 1 means no reconnection.
			//Default value is 100ms.
			int ivl{ 30000 };
			zmq_setsockopt(s, ZMQ_RECONNECT_IVL, &ivl, sizeof(int));

			if (zmq_connect(s, (boost::format("tcp://%s:%d") % ip % port).str().c_str()))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}

	return s;
}
