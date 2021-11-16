#include "boost/bind.hpp"
#include "boost/checked_delete.hpp"
#include "boost/functional/factory.hpp"
#include "boost/thread.hpp"

#include "libcommon/defs.h"
#include "libcommon/error.h"
#include "libcommon/hardware/cpu/cpu.h"
using Cpu = framework::libcommon::hardware::Cpu;
#include "libcommon/thread/threadpool.h"
using ThreadPool = framework::libcommon::ThreadPool;
#include "libcommon/lock/rwlock.h"
using WRLock = framework::libcommon::WRLock;
#include "ctx.h"
using namespace module::network::asio;

class ICtx
{
public:
	ICtx(void);
	~ICtx(void);

public:
	CommonError init(void);
	CommonError uninit(void);
	void* io(void);

private:
	IOContextGroup ioctxGroup;
	IOWorkGroup ioworkGroup;
	ThreadPool threadPool;
	int idleIONumber;
	WRLock wrLock;
};//class ICtx

ICtx::ICtx() 
	: ioctxGroup{Cpu().getCoreNumber()}, idleIONumber{ 0 }
{}
ICtx::~ICtx()
{
	uninit();
}

CommonError ICtx::init()
{
	for (int i = 0; i != ioctxGroup.size(); ++i)
	{
		void* t{
			threadPool.create(
				boost::bind(&boost::asio::io_context::run, boost::ref(ioctxGroup[i])))};

		if (t)
		{
			boost::thread* thr{reinterpret_cast<boost::thread*>(t)};
			Cpu().setAffinity(thr->native_handle(), i);
		}
		
		ioworkGroup.push_back(boost::asio::io_context::work(ioctxGroup[i]));
	}

	return CommonError::COMMON_ERROR_SUCCESS;
}

CommonError ICtx::uninit()
{
	std::for_each(ioctxGroup.begin(), ioctxGroup.end(), boost::bind(&boost::asio::io_context::stop, _1));
	threadPool.joinAll();
	ioworkGroup.clear();
	ioctxGroup.clear();
	return CommonError::COMMON_ERROR_SUCCESS;
}

void* ICtx::io()
{
	wrLock.wlock();
	boost::asio::io_context& c{
		ioctxGroup.at(idleIONumber++ % ioctxGroup.size())};
	wrLock.unwlock();

	return &c;
}

Ctx::Ctx() : ctx{boost::factory<ICtx*>()()}
{}

Ctx::~Ctx()
{
	boost::checked_delete(ctx);
}

int Ctx::init()
{
	return static_cast<int>(ctx ? ctx->init() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
}

int Ctx::uninit()
{
	return static_cast<int>(ctx ? ctx->uninit() : CommonError::COMMON_ERROR_BAD_NEW_INSTANCE);
}

void* Ctx::io()
{
	return ctx ? ctx->io() : nullptr;
}
