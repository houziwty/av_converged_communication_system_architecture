#include "error_code.h"
#include "io_service.h"
using namespace module::network::asio;

IoService::IoService() : idle{ 0 }
{}

IoService::~IoService()
{
	destroy();
}

boost::asio::io_context& IoService::getIdle()
{
	return ctxs.at(idle++ % ctxs.size());
}

int IoService::createNew(const int number/* = 1*/)
{
	for (int i = 0; i != number; ++i)
	{
		std::unique_ptr<boost::asio::io_context::work> work{
			new(std::nothrow) boost::asio::io_context::work(ctxs[i])};

		if(work)
		{
			works[i].swap(work);
		}
	}

	for (int j = 0; j != ctxs.size(); ++j)
	{
		threads.emplace_back([&](){ctxs[j].run();});
	}        

	return 0 < threads.size() ? Error_Code_Success : Error_Code_Operate_Failure;
}

int IoService::destroy()
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
