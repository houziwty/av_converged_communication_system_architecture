#include "boost/checked_delete.hpp"
#ifdef __cplusplus
extern "C" {
#endif
#include "zmq.h"
#ifdef __cplusplus
}
#endif
#include "utils/hardware/cpu.h"
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
		const int number{ Cpu().getCount() };
		zmq_ctx_set(c, ZMQ_IO_THREADS, number);
	}

	return c;
}

int Ctx::destroy(void* c/* = nullptr*/)
{
	int ret{Error_Code_Invalid_Param};

	if (c)
	{
		ret = (!zmq_ctx_term(c) ? Error_Code_Success : Error_Code_Bad_Operate_Term);
	}
	
	return ret;
}
