#include "boost/checked_delete.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "msg.h"
using namespace module::network::xmq;

Msg::Msg()
{}

Msg::~Msg()
{
	for (int i = 0; i != messages.size(); ++i)
	{
		boost::checked_delete(messages[i]);
	}
}

int Msg::add(const void* data /* = nullptr */, const uint64_t bytes /* = 0 */)
{
	//Allow empty
	int ret{data/* && 0 < bytes*/ ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		Any* any{ new(std::nothrow) Any };

		if (any)
		{
			any->input(data, bytes);
			messages.push_back(any);
		}
		else
		{
			ret = Error_Code_Bad_New_Object;
		}
	}

	return ret;
}

int Msg::recv(xsocket s /* = nullptr */)
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
				ret = add(zmq_msg_data(&msg), zmq_msg_size(&msg));

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

int Msg::send(xsocket s /* = nullptr */)
{
	int ret{ s ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		const std::size_t number{ messages.size() };
		zmq_msg_t msg;

		for (int i = 0; i != number; ++i)
		{
			const int sndflag{i < number - 1 ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT};
			const void* data{messages[i]->data()};
			const uint64_t bytes{messages[i]->bytes()};

			if (zmq_msg_init_size(&msg, bytes))
			{
				return Error_Code_Bad_New_Object;
			}

			//必须拷贝数据到消息内
			XMem().copy(data, bytes, zmq_msg_data(&msg), zmq_msg_size(&msg));
			int ret = zmq_msg_send(&msg, s, sndflag);
			if (bytes != ret)
			{
				ret = Error_Code_Bad_Operate_Send;
				break;
			}
		}

		zmq_msg_close(&msg);
	}

	return ret;
}
