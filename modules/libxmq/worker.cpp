#include "zmq.h"
#include "error_code.h"
#include "xmq/ctx.h"
#include "xmq/dealer.h"
#include "xmq/msg.h"
#include "worker.h"
using namespace module::network::xmq;

Worker::Worker(PolledDataWithoutIDCallback callback) 
	: ctx{nullptr}, dealer{nullptr}, handler{callback}
{}

Worker::~Worker()
{
	Dealer().shutdown(dealer);
	Ctx().destroy(ctx);
}

int Worker::connect(
	const void* uid/* = nullptr*/, 
	const int uid_bytes/* = 0*/, 
	const char* ip/* = nullptr*/, 
	const unsigned short port /* = 0 */)
{
	if(ctx)
	{
		return Error_Code_Object_Existed;
	}

	int ret{uid && 0 < uid_bytes && ip && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ctx = Ctx().createNew();

		if(ctx)
		{
			dealer = Dealer().connect(uid, uid_bytes, ip, port, ctx);

			if(!dealer)
			{
				Ctx().destroy(ctx);
				ctx = nullptr;
			}
		}
	}

	return ctx && dealer ? Error_Code_Success : Error_Code_Bad_Operate_Bind;
}

int Worker::send(const void* data/* = nullptr*/, const int bytes/* = 0*/)
{
	int ret{ctx && dealer ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		ret = (data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			msg.append("", 0);
			msg.append(data, bytes);
			ret = msg.send(dealer);
		}
	}

	return ret;
}

int Worker::poll()
{
	int ret{dealer ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		zmq_pollitem_t pollitems[]{ { dealer, 0, ZMQ_POLLIN, 0} };
		zmq_poll(pollitems, 1, 1);

		if (pollitems[0].revents & ZMQ_POLLIN)
		{
			Msg msg;
			ret = msg.recv(dealer);
			if (Error_Code_Success == ret)
			{
				//只使用第二个数据
				if (handler)
				{
					handler(msg.msg(1), msg.msg_bytes(1));
				}
			}
		}
	}

	return ret;
}
