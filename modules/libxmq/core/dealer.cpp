#include "boost/format.hpp"
#include "zmq.h"
#include "error_code.h"
#include "dealer.h"
using namespace module::network::xmq;

Dealer::Dealer() : Sock()
{}

Dealer::~Dealer()
{}

void* Dealer::connect(
	void* ctx /* = nullptr */, 
	const char* name/* = nullptr*/, 
	const char* ip/* = nullptr*/, 
	const uint16_t port /* = 0 */, 
	const uint64_t hwm /* = 0 */, 
	const uint64_t reconivl /* = 0 */)
{
	void* s{nullptr};
	const std::string id{name};

	if (ctx && name && ip && 0 < port)
	{
		s = zmq_socket(ctx, ZMQ_DEALER);

		if (s)
		{
			int keepalive{ 1 };//, idle{30};
			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE, &keepalive, sizeof(int));
			//zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE_IDLE, &idle, sizeof(int));
			zmq_setsockopt(s, ZMQ_IDENTITY, id.c_str(), id.length());

			//The default value is 1000.
			if (0 < hwm)
			{
				zmq_setsockopt(s, ZMQ_RCVHWM, &hwm, sizeof(const uint64_t));
				zmq_setsockopt(s, ZMQ_SNDHWM, &hwm, sizeof(const uint64_t));
			}

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

			//Set reconnection interval.
			//The ZMQ_RECONNECT_IVL option shall set the initial reconnection interval for the specified socket.
			//The reconnection interval is the period ØMQ shall wait between attempts to reconnect disconnected peers when using connection - oriented transports.
			//The value - 1 means no reconnection.
			//Default value is 100ms.
			int ivl{ 100 };
			zmq_setsockopt(s, ZMQ_RECONNECT_IVL, &ivl, sizeof(int));

			//Maximum time before a recv/send operation returns with EAGAIN.
			//Sets the timeout for receive operation on the socket.
			//*	If the value is 0, zmq_recv(3)/zmq_send(3) will return immediately, with a EAGAIN error if there is no message to receive.
			//*	If the value is - 1, it will block until a message is available.
			//*	For all other values, it will wait for a message for that amount of time before returning with an EAGAIN error.
			int tmo{ 0 };
			zmq_setsockopt(s, ZMQ_RCVTIMEO, &tmo, sizeof(int));
			zmq_setsockopt(s, ZMQ_SNDTIMEO, &tmo, sizeof(int));

			if (zmq_connect(s, (boost::format("tcp://%s:%d") % ip % port).str().c_str()))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}

	return s;
}

void* Dealer::bind(
	void* ctx /* = nullptr */, 
	const char* name/* = nullptr*/, 
	const uint64_t hwm /* = 0 */)
{
	void* s{nullptr};

	if (ctx && name)
	{
		s = zmq_socket(ctx, ZMQ_DEALER);

		if (s)
		{
			//The default value is 1000.
			if (0 < hwm)
			{
				zmq_setsockopt(s, ZMQ_RCVHWM, &hwm, sizeof(const uint64_t));
				zmq_setsockopt(s, ZMQ_SNDHWM, &hwm, sizeof(const uint64_t));
			}

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
			
			if (zmq_bind(s, name))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}
	
	return s;
}

void* Dealer::connect_inproc(
	void* ctx /* = nullptr */, 
	const char* name/* = nullptr*/, 
	const uint64_t hwm /* = 0 */)
{
	void* s{nullptr};

	if (ctx)
	{
		s = zmq_socket(ctx, ZMQ_DEALER);

		if (s)
		{
			int keepalive{ 1 };
			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE, &keepalive, sizeof(int));

			//The default value is 1000.
			if (0 < hwm)
			{
				zmq_setsockopt(s, ZMQ_RCVHWM, &hwm, sizeof(const uint64_t));
				zmq_setsockopt(s, ZMQ_SNDHWM, &hwm, sizeof(const uint64_t));
			}

			//Set reconnection interval.
			//The ZMQ_RECONNECT_IVL option shall set the initial reconnection interval for the specified socket.
			//The reconnection interval is the period ØMQ shall wait between attempts to reconnect disconnected peers when using connection - oriented transports.
			//The value - 1 means no reconnection.
			//Default value is 100ms.
			int ivl{ 100 };
			zmq_setsockopt(s, ZMQ_RECONNECT_IVL, &ivl, sizeof(int));

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

			if(zmq_connect(s, name))
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}
	
	return s;
}
