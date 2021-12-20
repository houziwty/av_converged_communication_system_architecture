#include "zmq.h"
#include "error_code.h"
#include "xmq/ctx.h"
#include "xmq/sub.h"
#include "xmq/msg.h"
#include "subscriber.h"
using namespace module::network::xmq;

Subscriber::Subscriber(PolledDataWithoutIDCallback callback) 
	: ctx{nullptr}, sub{nullptr}, handler{callback}
{}

Subscriber::~Subscriber()
{
	Sub().shutdown(sub);
	Ctx().get_mutable_instance().destroy(ctx);
}

int Subscriber::connect(
	const std::string ip, 
	const unsigned short port /* = 0 */,
	const int hwm /* = 10 */)
{
	if(ctx)
	{
		return Error_Code_Object_Existed;
	}

	int ret{!ip.empty() && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ctx = Ctx().get_mutable_instance().createNew();

		if(ctx)
		{
			sub = Sub().connect(ip, port, ctx);

			if(!sub)
			{
				Ctx().get_mutable_instance().destroy(ctx);
				ctx = nullptr;
			}
		}
	}

	return ctx && sub ? Error_Code_Success : Error_Code_Bad_Operate_Bind;
}

int Subscriber::poll()
{
	int ret{sub ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		zmq_pollitem_t pollitems[]{ { sub, 0, ZMQ_POLLIN, 0} };
		zmq_poll(pollitems, 1, 1);

		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			Msg msg;
			ret = msg.recv(sub);
			if (Error_Code_Success == ret)
			{
				if (handler)
				{
					handler(msg.popFront());
				}
			}
		}
	}

	return ret;
}
