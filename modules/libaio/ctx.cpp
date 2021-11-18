#include "error_code.h"
#include "ctx.h"

MultiService::MultiService(
	const std::size_t number/* = std::thread::hardware_concurrency()*/) 
	: ctxs{number}, works{number}, idle{ 0 }
{
	createNew(number);
}

MultiService::~MultiService()
{
	uninit();
}

boost::asio::io_context& MultiService::getIdle()
{
//	wrLock.wlock();
	auto& ctx{ctxs.at(idle++ % ctxs.size())};
//	wrLock.unwlock();

	return ctx;
}

int MultiService::createNew(const std::size_t number/* = 1*/)
{
	for (int i = 0; i != number; ++i)
	{
		std::unique_ptr<boost::asio::io_context::work> work{
			new(std::nothrow) boost::asio::io_context::work(ctxs[i])};

		if(work)
		{
			works[i] = work;
		}
	}

	for (int j = 0; j != ctxs.size(); ++j)
	{
		threads.emplace_back([this, i](){ctxs[i].run();});
	}        

	return 0 < threads.size() ? Error_Code_Success : Error_Code_Operate_Failure;
}

int MultiService::destory()
{
	for (auto& work : works)
	{
		work.reset();
	}

	for (auto& thread : threads)
	{
		thread.join();
	}

	return Error_Code_Success;
}
