#include "zmq.h"
#include "error_code.h"
#include "xmq/ctx.h"
#include "xmq/router.h"
#include "xmq/msg.h"
#include "switcher.h"
using namespace module::network::xmq;

Switcher::Switcher(PolledDataWithIDCallback callback /*= nullptr*/) 
	: ctx{nullptr}, router{nullptr}, handler{callback}
{}

Switcher::~Switcher()
{
	Router().shutdown(router);
	Ctx().get_mutable_instance().destroy(ctx);
}

int Switcher::bind(const unsigned short port /* = 0 */)
{
	if(ctx)
	{
		return Error_Code_Object_Existed;
	}

	int ret{0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ctx = Ctx().get_mutable_instance().createNew();

		if(ctx)
		{
			router = Router().bind(ctx, port);

			if(!router)
			{
				Ctx().get_mutable_instance().destroy(ctx);
				ctx = nullptr;
			}
		}
	}

	return ctx && router ? Error_Code_Success : Error_Code_Bad_Operate_Bind;
}

int Switcher::send(
	const std::string uid, 
	const std::string data)
{
	int ret{ctx && router ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		ret = (!uid.empty() && !data.empty() ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			msg.pushBack(uid);
			msg.pushBack("");
			msg.pushBack(data);

			ret = msg.send(router);
		}
	}

	return ret;
}

int Switcher::poll()
{
	int ret{router ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		zmq_pollitem_t pollitems[]{ { router, 0, ZMQ_POLLIN, 0} };
		zmq_poll(pollitems, 1, 1);

		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			Msg msg;
			ret = msg.recv(router);
			if (Error_Code_Success == ret)
			{
				const std::string uid{msg.popFront()};
				msg.popFront();
				const std::string data{msg.popFront()};
				
				if (handler)
				{
					handler(uid, data);
				}
			}
		}
	}

	return ret;
}
