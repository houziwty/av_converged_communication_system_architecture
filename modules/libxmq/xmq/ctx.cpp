#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "hardware/cpu.h"
using namespace framework::utils::hardware;
#include "error_code.h"
#include "ctx.h"
using namespace module::network::xmq;

Ctx::Ctx()
{}

Ctx::~Ctx()
{}

xctx Ctx::createNew()
{
	xctx c{zmq_ctx_new()};

	if (c)
	{
		const int number{ Cpu().getCount() };
		zmq_ctx_set(c, ZMQ_IO_THREADS, number);
	}

	return c;
}

int Ctx::destroy(xctx c/* = nullptr*/)
{
	int ret{Error_Code_Invalid_Param};

	if (c)
	{
		ret = (!zmq_ctx_shutdown(c) ? Error_Code_Success : Error_Code_Bad_Operate_Term);
	}
	
	return ret;
}
