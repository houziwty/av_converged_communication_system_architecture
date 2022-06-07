#include "boost/format.hpp"
#include "zmq.h"
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
			int keepalive{ 1 };
//			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE, &keepalive, sizeof(int));

			//The default value is 1000.
			if (0 < hwm)
			{
				zmq_setsockopt(s, ZMQ_RCVHWM, &hwm, sizeof(const uint64_t));
				zmq_setsockopt(s, ZMQ_SNDHWM, &hwm, sizeof(const uint64_t));
			}

			// int handover{ 1 };
			// zmq_setsockopt(s, ZMQ_ROUTER_HANDOVER, &handover, sizeof(int));

			//Retrieve linger period for socket shutdown.
			//* The default value of - 1 specifies an infinite linger period.
			//	Pending messages shall not be discarded after a call to zmq_close(); 
			//	attempting to terminate the socket's context with zmq_term() shall block until all pending messages have been sent to a peer.
			//*	The value of 0 specifies no linger period.Pending messages shall be discarded immediately when the socket is closed with zmq_close().
			//*	Positive values specify an upper bound for the linger period in milliseconds.
			//	Pending messages shall not be discarded after a call to zmq_close(); 
			//	attempting to terminate the socket's context with zmq_term() shall block until either all pending messages have been sent to a peer, or the linger period expires, after which any pending messages shall be discarded.
			int linger{ 0 };
			zmq_setsockopt(s, ZMQ_LINGER, &linger, sizeof(int));

			//Maximum time before a recv/send operation returns with EAGAIN.
			//Sets the timeout for receive operation on the socket.
			//*	If the value is 0, zmq_recv(3)/zmq_send(3) will return immediately, with a EAGAIN error if there is no message to receive.
			//*	If the value is - 1, it will block until a message is available.
			//*	For all other values, it will wait for a message for that amount of time before returning with an EAGAIN error.
			int tmo{ 0 };
			zmq_setsockopt(s, ZMQ_RCVTIMEO, &tmo, sizeof(int));
			zmq_setsockopt(s, ZMQ_SNDTIMEO, &tmo, sizeof(int));
			
			if(zmq_bind(s, (boost::format("tcp://*:%d") % port).str().c_str()))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}

	return s;
}
