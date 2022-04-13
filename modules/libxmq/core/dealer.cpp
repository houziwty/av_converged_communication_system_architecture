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
			int keepalive{ 1 }, idle{30};
			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE, &keepalive, sizeof(int));
			zmq_setsockopt(s, ZMQ_TCP_KEEPALIVE_IDLE, &idle, sizeof(int));
			zmq_setsockopt(s, ZMQ_IDENTITY, id.c_str(), id.length());

			if (0 < hwm)
			{
				zmq_setsockopt(s, ZMQ_RCVHWM, &hwm, sizeof(const uint64_t));
				zmq_setsockopt(s, ZMQ_SNDHWM, &hwm, sizeof(const uint64_t));
			}

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
			if (0 < hwm)
			{
				zmq_setsockopt(s, ZMQ_RCVHWM, &hwm, sizeof(const uint64_t));
				zmq_setsockopt(s, ZMQ_SNDHWM, &hwm, sizeof(const uint64_t));
			}
			
			zmq_bind(s, name);
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
			if (0 < hwm)
			{
				zmq_setsockopt(s, ZMQ_RCVHWM, &hwm, sizeof(const uint64_t));
				zmq_setsockopt(s, ZMQ_SNDHWM, &hwm, sizeof(const uint64_t));
			}

			int ret{zmq_connect(s, name)};

			if(ret)
			{
				zmq_close(s);
				s = nullptr;
			}
		}
	}
	
	return s;
}
