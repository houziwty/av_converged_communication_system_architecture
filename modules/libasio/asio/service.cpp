#include "error_code.h"
#include "time/xtime.h"
using namespace framework::utils::time;
#include "service.h"
using namespace module::network::asio;

Service::Service() 
	: idle{ 0 }, works{ ctxs.size() }, 
	ctxs{std::thread::hardware_concurrency()}
{}

Service::~Service()
{
	stop();
}

int Service::run()
{
	const size_t number{ ctxs.size() };

	for (int i = 0; i != number; ++i)
	{
		ctxs[i] = new(std::nothrow) boost::asio::io_context();

		if (ctxs[i])
		{
			works[i] = new(std::nothrow) boost::asio::io_context::work(*ctxs[i]);
			boost::thread* thread{ new(std::nothrow) boost::thread([&, i]() {ctxs[i]->run(); }) };

			if (thread)
			{
				threads.push_back(thread);
			}
		}
	}

	return 0 < threads.size() ? Error_Code_Success : Error_Code_Bad_New_Thread;
}

int Service::stop()
{
	for (auto& work : works)
	{
		boost::checked_delete(work);
	}

	for (auto& ctx : ctxs)
	{
		if (ctx)
		{
			ctx->stop();
			XTime().sleep(20);
		}
		
		boost::checked_delete(ctx);
	}

	for (auto& thread : threads)
	{
		//Forbidden waiting
		if (thread)
		{
			thread->join();
		}

		boost::checked_delete(thread);
	}

	ctxs.clear();
	works.clear();
	threads.clear();

	return Error_Code_Success;
}

boost::asio::io_context* Service::ctx()
{
	return ctxs.at(idle++ % ctxs.size());
}
