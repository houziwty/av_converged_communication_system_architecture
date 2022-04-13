#include <string>
#include "zmq.h"
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "sock.h"
using namespace module::network::xmq;

Sock::Sock()
{}

Sock::~Sock()
{}

int Sock::shutdown(void* s /* = nullptr */)
{
	int ret{s ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		ret = (!zmq_close(s) ? Error_Code_Success : Error_Code_Operate_Failure);
	}
	
	return ret;
}

int Sock::recv(
	void* s, 
	void* buffer, 
	const uint64_t bytes, 
	bool& more)
{
	int recvbytes{0};

	if (s && buffer && 0 < bytes)
	{
		zmq_msg_t msg;

		if (!zmq_msg_init(&msg))
		{
			//Block for recieving data.
			recvbytes = zmq_msg_recv(&msg, s, 0);
			XMem().copy(zmq_msg_data(&msg), recvbytes, buffer, bytes);
			more = (0 < zmq_msg_more(&msg) ? true : false);
		}

		zmq_msg_close(&msg);
	}

	return recvbytes;
}

int Sock::send(
	void* s /* = nullptr */, 
	const void* buffer /* = nullptr */, 
	const uint64_t bytes /* = 0 */, 
	bool more /* = false */)
{
	int sentbytes{0};

	//Enable bytes == 0
	if (s && buffer)
	{
		zmq_msg_t msg;

		if (!zmq_msg_init_size(&msg, bytes))
		{
			//Block for sending data.
			XMem().copy(buffer, bytes, zmq_msg_data(&msg), bytes);
			int ret = zmq_msg_send(&msg, s, more ? ZMQ_SNDMORE : 0);
			if (0 < ret)
			{
				sentbytes = ret;
			}
		}

		zmq_msg_close(&msg);
	}

	return sentbytes;
}

int Sock::forward(void* rs /* = nullptr */, void* ss /* = nullptr */)
{
	int ret{ rs && ss ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		bool more{ true };

		while (more)
		{
			zmq_msg_t msg;

			if (zmq_msg_init(&msg))
			{
				return Error_Code_Bad_New_Object;
			}

			if (-1 < zmq_msg_recv(&msg, rs, ZMQ_DONTWAIT))
			{
				more = (0 < zmq_msg_more(&msg) ? true : false);
				ret = zmq_msg_send(&msg, ss, more ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT);
			}

			zmq_msg_close(&msg);
		}
	}

	return ret;
}