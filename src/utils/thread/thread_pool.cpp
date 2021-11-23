#include "error_code.h"
#include "utils/thread/thread_pool.h"
using namespace framework::utils::thread;

ThreadPool::ThreadPool()
{}

ThreadPool::~ThreadPool()
{}

thread_t* ThreadPool::createNew(Function f)
{
	return pool.create_thread(f);
}

int ThreadPool::destroy(thread_t* t /* = nullptr */)
{
	int ret{Error_Code_Invalid_Param};

	if (t)
	{
		boost::thread* pthread{ reinterpret_cast<boost::thread*>(t) };

		if (pool.is_thread_in(pthread))
		{
			pool.remove_thread(pthread);
			ret = Error_Code_Success;
		}
		else
		{
			ret = Error_Code_Operate_Not_Support;
		}
	}

	return ret;
}
