#include "boost/checked_delete.hpp"
#include "libzmq/zmq.h"
#include "utils/hw/cpu.h"
using namespace utils::hw;
#include "error_code.h"
#include "network/xmq/ctx.h"
using namespace network::xmq;

Ctx::Ctx()
{}

Ctx::~Ctx()
{}

ctx_t* Ctx::createNew()
{
	ctx_t* c{zmq_ctx_new()};

	if (c)
	{
		const size_t number{ Cpu().getCount() };
		zmq_ctx_set(c, ZMQ_IO_THREADS, number);
	}

	return c;
}

int Ctx::destroy(ctx_t* c/* = nullptr*/)
{
	int ret{Error_Code_Invalid_Param};

	if (c)
	{
		ret = (!zmq_ctx_term(c) ? Error_Code_Success : Error_Code_Bad_Operate_Term);
	}
	
	return ret;
}
