#include <cstring>
#include "boost/checked_delete.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "utils/memory/xstr.h"
using namespace framework::utils::memory;
#include "msg.h"
using namespace module::network::xmq;

Msg::Msg() : counter{0}
{
	memset(msgs, 0, Max * sizeof(int));
}

Msg::~Msg()
{
	clear();
}

int Msg::append(const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
	int ret{counter < (Max / 2) ? Error_Code_Success : Error_Code_Operate_Out_Of_Range};

	if (Error_Code_Success == ret)
	{
		if (data && 0 < bytes)
		{
			const char* buf{
				XStr().copyNew(reinterpret_cast<const char*>(data), bytes)};

			if (buf)
			{
				msgs[counter] = bytes;
				msgs[counter + 1] = (int)buf;
				++counter;
			}
			else
			{
				ret = Error_Code_Bad_New_Memory;
			}
		}
		else
		{
			ret = Error_Code_Invalid_Param;
		}
	}

	return ret;
}

void Msg::clear()
{
	for (int i = 0; i != counter; ++i)
	{
		boost::checked_array_delete((void*)msgs[i + 1]);
	}
}

int Msg::recv(socket_t s /* = nullptr */)
{
	int ret{ s ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		zmq_msg_t msg;

		while (1)
		{
			if (zmq_msg_init(&msg))
			{
				return Error_Code_Bad_New_Object;
			}

			if (-1 < zmq_msg_recv(&msg, s, ZMQ_DONTWAIT))
			{
				ret = append(reinterpret_cast<char*>(zmq_msg_data(&msg)), zmq_msg_size(&msg));
				if (!zmq_msg_more(&msg) || Error_Code_Success != ret)
				{
					break;
				}
			}

			zmq_msg_close(&msg);
		}
	}

	return ret;
}

int Msg::send(void* s /* = nullptr */)
{
	int ret{ s ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		zmq_msg_t msg;

		for (int i = 0; i != counter; ++i)
		{
			const int sndflag{i < counter - 1 ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT};

			if (zmq_msg_init_size(&msg, msgs[i]))
			{
				return Error_Code_Bad_New_Object;
			}

			XStr().copy(
				(const char*)msgs[i + 1], 
				msgs[i], 
				reinterpret_cast<char*>(zmq_msg_data(&msg)), 
				msgs[i]);
				
			if (msgs[i] != zmq_msg_send(&msg, s, sndflag))
			{
				ret = Error_Code_Bad_Operate_Send;
				break;
			}
		}

		zmq_msg_close(&msg);
	}

	return ret;
}
