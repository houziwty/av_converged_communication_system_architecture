#include "libzmq/zmq.h"
#include "error_code.h"
#include "network/xmq/msg.h"
using namespace network::xmq;

Msg::Msg()
{}

Msg::~Msg()
{}

void Msg::pushBack(const std::string data)
{
	msgs.push_bask(data);
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

int Msg::recv(socket_t* s /* = nullptr */)
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

	return e;
}

int Msg::sendData(socket_t* s /* = nullptr */)
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

			if (len != zmq_msg_send(&msg, s, i < msgNo - 1 ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT))
			{
				ret = Error_Code_Bad_Operate_Send;
				break;
			}
		}
	}

	return ret;
}
