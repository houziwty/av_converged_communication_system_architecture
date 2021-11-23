#include "zmq.h"
#include "error_code.h"
#include "xmq/ctx.h"
#include "xmq/dealer.h"
#include "xmq/msg.h"
#include "worker.h"
using namespace module::network::xmq;

Worker::Worker(PolledDataWithoutIDCallback callback /*= nullptr*/) 
	: ctx{nullptr}, dealer{nullptr}, handler{callback}
{}

Worker::~Worker()
{
	Dealer().shutdown(dealer);
	Ctx().get_mutable_instance().destroy(ctx);
}

int Worker::connect(
	const std::string uid,
	const std::string ip, 
	const unsigned short port /* = 0 */)
{
	if(ctx)
	{
		return Error_Code_Object_Existed;
	}

	int ret{!uid.empty() && !ip.empty() && 0 < port ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ctx = Ctx().get_mutable_instance().createNew();

		if(ctx)
		{
			dealer = Dealer().connect(uid, ip, port, ctx);

			if(!dealer)
			{
				Ctx().get_mutable_instance().destroy(ctx);
				ctx = nullptr;
			}
		}
	}

	return ctx && dealer ? Error_Code_Success : Error_Code_Bad_Operate_Bind;
}

int Worker::send(const std::string data)
{
	int ret{ctx && dealer ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		ret = (!data.empty() ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			msg.pushBack("");
			msg.pushBack(data);

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
				msg.popFront();
				const std::string data{msg.popFront()};
				
				if (handler)
				{
					handler(data);
				}
			}
		}
	}

	return ret;
}
