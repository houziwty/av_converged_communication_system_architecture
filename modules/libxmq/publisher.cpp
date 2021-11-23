#include "zmq.h"
#include "error_code.h"
#include "xmq/ctx.h"
#include "xmq/pub.h"
#include "xmq/msg.h"
#include "publisher.h"
using namespace module::network::xmq;

Publisher::Publisher() : ctx{nullptr}, pub{nullptr}
{}

Publisher::~Publisher()
{
	Pub().shutdown(pub);
	Ctx().get_mutable_instance().destroy(ctx);
}

int Publisher::bind(
	const unsigned short port /* = 0 */,
	const int hwm/* = 10*/)
{
	if(ctx)
	{
		return Error_Code_Object_Existed;
	}
		
	int ret{0 < port && 0 < hwm ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ctx = Ctx().get_mutable_instance().createNew();

		if(ctx)
		{
			pub = Pub().bind(ctx, port, hwm);

			if(!pub)
			{
				Ctx().get_mutable_instance().destroy(ctx);
				ctx = nullptr;
			}
		}
	}

	return ctx && pub ? Error_Code_Success : Error_Code_Bad_Operate_Bind;
}

int Publisher::send(const std::string data)
{
	int ret{ctx && pub ? Error_Code_Success : Error_Code_Operate_Failure};

	if(Error_Code_Success == ret)
	{
		ret = (!data.empty() ? Error_Code_Success : Error_Code_Invalid_Param);

		if(Error_Code_Success == ret)
		{
			Msg msg;
			msg.pushBack(data);

			ret = msg.send(pub);
		}
	}

	return ret;
}
