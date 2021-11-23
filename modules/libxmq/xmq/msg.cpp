#include <cstring>
#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "error_code.h"
#include "msg.h"
using namespace module::network::xmq;

Msg::Msg()
{}

Msg::~Msg()
{}

void Msg::pushBack(const std::string data)
{
	msgs.push_back(data);
}

const std::string Msg::popFront()
{
	std::string data;

	if (0 < msgs.size())
	{
		data = msgs.front();
		msgs.erase(msgs.begin());
	}

	return std::move(data);
}

int Msg::recv(void* s /* = nullptr */)
{
	int ret{ s ? Error_Code_Success : Error_Code_Invalid_Param };
	zmq_msg_t msg;

	while (Error_Code_Success == ret)
	{
		if (zmq_msg_init(&msg))
		{
			return Error_Code_Bad_New_Object;
		}
		
		if (!zmq_msg_recv(&msg, s, ZMQ_DONTWAIT))
		{
			msgs.push_back(std::string(reinterpret_cast<char*>(zmq_msg_data(&msg)), zmq_msg_size(&msg)));
			if (!zmq_msg_more(&msg))
			{
				break;
			}
		}

		zmq_msg_close(&msg);
	}

	return ret;
}

int Msg::send(void* s /* = nullptr */)
{
	int ret{ s ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		const size_t number{ msgs.size() };

		for (int i = 0; i != number; ++i)
		{
			zmq_msg_t msg;

			if (zmq_msg_init(&msg))
			{
				return Error_Code_Bad_New_Object;
			}

			const std::string data{ msgs[i] };
			const size_t len{ data.length() };
#ifdef OS_WINDOWS
			memcpy_s(zmq_msg_data (&msg), len, data.c_str(), len);
#else
			memcpy(zmq_msg_data (&msg), data.c_str(), len);
#endif//OS_WINDOWS

			if (len != zmq_msg_send(&msg, s, i < number - 1 ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT))
			{
				ret = Error_Code_Bad_Operate_Send;
				break;
			}
		}
	}

	return ret;
}
