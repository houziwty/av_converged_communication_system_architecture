#include "boost/checked_delete.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "msg.h"
using namespace module::network::xmq;

Msg::Msg()
{}

Msg::~Msg()
{
	clear();
}

int Msg::append(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	void* buf{ XMem().copyNew(data, bytes) };
	messages.push_back(Message{ const_cast<void*>(buf), bytes });
	return Error_Code_Success;
}

void Msg::clear()
{
	for (int i = 0; i != messages.size(); ++i)
	{
		boost::checked_array_delete(
			reinterpret_cast<const char*>(messages[i].data));
	}
}

int Msg::recv(socket_t s /* = nullptr */)
{
	int ret{ s ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		bool more{true};

		while (more)
		{
			zmq_msg_t msg;
			
			if (zmq_msg_init(&msg))
			{
				return Error_Code_Bad_New_Object;
			}

			if (-1 < zmq_msg_recv(&msg, s, ZMQ_DONTWAIT))
			{
				ret = append(zmq_msg_data(&msg), (const int)zmq_msg_size(&msg));
				if (!zmq_msg_more(&msg))
				{
					more = false;
				}
			}

			zmq_msg_close(&msg);
		}
	}

	return ret;
}

int Msg::send(socket_t s /* = nullptr */)
{
	int ret{ s ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		const std::size_t number{ messages.size() };
		zmq_msg_t msg;

		for (int i = 0; i != number; ++i)
		{
			const int sndflag{i < number - 1 ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT};

			if (zmq_msg_init_size(&msg, messages[i].bytes))
			{
				return Error_Code_Bad_New_Object;
			}

			//必须拷贝数据到消息内
			XMem().copy(
				messages[i].data, 
				messages[i].bytes, 
				zmq_msg_data(&msg), 
				zmq_msg_size(&msg));

			if (messages[i].bytes != zmq_msg_send(&msg, s, sndflag))
			{
				ret = Error_Code_Bad_Operate_Send;
				break;
			}
		}

		zmq_msg_close(&msg);
	}

	return ret;
}
