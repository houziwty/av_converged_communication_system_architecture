#include "error_code.h"
#include "utils/thread/thread_pool.h"
using namespace utils::thread;

ThreadPool::ThreadPool()
{}

ThreadPool::~ThreadPool()
{
	//销毁线程池实例时要求所有该实例创建的线程都已结束运行
	pool.join_all();
}

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

		if (tg.is_thread_in(pthread))
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
