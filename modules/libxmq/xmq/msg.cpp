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

Msg::Msg() 
	: addressData{nullptr}, addressBytes{0}, contentData{nullptr}, contentBytes{0}
{}

Msg::~Msg()
{
	clear();
}

int Msg::address(const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ret = XStr().copy(
			reinterpret_cast<const char*>(data), 
			bytes, 
			reinterpret_cast<char*>(addressData), 
			bytes);
	}
	
	return ret;
}

int Msg::content(const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ret = XStr().copy(
			reinterpret_cast<const char*>(data), 
			bytes, 
			reinterpret_cast<char*>(contentData), 
			bytes);
	}
	
	return ret;
}

void Msg::clear()
{
	boost::checked_array_delete(addressData);
	boost::checked_array_delete(contentData);
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
				msgs.push_back(std::string(reinterpret_cast<char*>(zmq_msg_data(&msg)), zmq_msg_size(&msg)));
				if (!zmq_msg_more(&msg))
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
		const size_t number{ msgs.size() };
		zmq_msg_t msg;

		for (int i = 0; i != number; ++i)
		{
			const int sndflag{i < number - 1 ? ZMQ_DONTWAIT | ZMQ_SNDMORE : ZMQ_DONTWAIT};
			const std::string& data{ msgs[i] };
			const size_t len{ data.length() };

			if (zmq_msg_init_size(&msg, len))
			{
				return Error_Code_Bad_New_Object;
			}

#ifdef WINDOWS
			memcpy_s(zmq_msg_data(&msg), len, data.c_str(), len);
#else
			memcpy(zmq_msg_data(&msg), data.c_str(), len);
#endif//WINDOWS

			if (len != zmq_msg_send(&msg, s, sndflag))
			{
				ret = Error_Code_Bad_Operate_Send;
				break;
			}
		}

		zmq_msg_close(&msg);
	}

	return ret;
}
