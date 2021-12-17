#include <algorithm>
#include "error_code.h"
#include "service.h"
using namespace module::network::asio;

Service::Service() 
	: idle{ 0 }, ctxs(std::max(static_cast<int>(std::thread::hardware_concurrency()), 1)), works(ctxs.size())
{}

Service::~Service()
{
	stop();
}

boost::asio::io_context& Service::ctx()
{
	return ctxs.at(idle++ % ctxs.size());
}

int Service::start()
{
	const int cpuNumber{static_cast<const int>(ctxs.size())};

	for (int i = 0; i != cpuNumber; ++i)
	{
		std::unique_ptr<boost::asio::io_context::work> work{
			new(std::nothrow) boost::asio::io_context::work(ctxs[i])};

		if(work)
		{
			works[i].swap(work);
		}
	}

	for (int j = 0; j != cpuNumber; ++j)
	{
		threads.emplace_back([&](){ctxs[j].run();});
	}        

	return 0 < threads.size() ? Error_Code_Success : Error_Code_Operate_Failure;
}

int Service::stop()
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
