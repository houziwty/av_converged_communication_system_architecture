#include "zmq.h"
#include "error_code.h"
#include "xmq/ctx.h"
#include "xmq/router.h"
#include "xmq/msg.h"
#include "switcher.h"
using namespace module::network::xmq;

Switcher::Switcher(PolledDataWithIDCallback callback) 
	: ctx{nullptr}, router{nullptr}, handler{callback}
{}

Switcher::~Switcher()
{
	Router().shutdown(router);
	Ctx().destroy(ctx);
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
		ctx = Ctx().createNew();

		if(ctx)
		{
			router = Router().bind(ctx, port);

			if(!router)
			{
				Ctx().destroy(ctx);
				ctx = nullptr;
			}
		}
	}

	return ctx && router ? Error_Code_Success : Error_Code_Bad_Operate_Bind;
}

int Switcher::send(
	const void* uid/* = nullptr*/, 
	const int uid_bytes/* = 0*/, 
	const void* data/* = nullptr*/, 
	const int data_bytes/* = 0*/)
{
	int ret{ctx && router ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		ret = (uid && 0 < uid_bytes && data && 0 < data_bytes ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			msg.append(uid, uid_bytes);
			msg.append("", 0);
			msg.append(data, data_bytes);
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
				//只读第一和第三段数据
				const void* uid{msg.msg()};
				const int uid_bytes{msg.msg_bytes()};
				const void* data{msg.msg(2)};
				const int data_bytes{msg.msg_bytes(2)};
				
				if (handler)
				{
					handler(uid, uid_bytes, data, data_bytes);
				}
			}
		}
	}

	return ret;
}
