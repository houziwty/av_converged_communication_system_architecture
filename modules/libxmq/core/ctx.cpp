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

void* Ctx::createNew()
{
	void* c{zmq_ctx_new()};

	if (c)
	{
		zmq_ctx_set(c, ZMQ_IO_THREADS, Cpu().getCount());
	}

	return c;
}

int Ctx::destroy(void* c/* = nullptr*/)
{
	int ret{c ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ret = (!zmq_ctx_shutdown(c) ? Error_Code_Success : Error_Code_Bad_Operate_Term);
	}
	
	return ret;
}
