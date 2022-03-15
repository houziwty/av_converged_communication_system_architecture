#include "boost/thread.hpp"
#ifdef _WINDOWS
#include <windows.h>
#include "boost/winapi/system.hpp"
#else
#include <pthread.h>
#endif//_WINDOWS
#include "error_code.h"
#include "thread/thread.h"
using namespace framework::utils::thread;

Thread::Thread()
{}

Thread::~Thread()
{}

int Thread::setAffinity(thread_t* t/* = nullptr*/, const int idx /* = 0 */)
{
	int ret{t ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		boost::thread* thread{reinterpret_cast<boost::thread*>(t)};

#ifdef _WINDOWS
		if(-1 < idx)
		{
			DWORD mask{static_cast<DWORD>(0x01 << idx)};
			ret = (!SetThreadAffinityMask(thread->native_handle(), mask) ? Error_Code_Operate_Failure : Error_Code_Success);
		}
#else
		cpu_set_t param;
		CPU_ZERO(&param);
		if (-1 < idx)
		{
			CPU_SET(idx, &param);
		}
		ret = (!pthread_setaffinity_np(thread->native_handle(), sizeof(param), &param) ? Error_Code_Success : Error_Code_Operate_Failure);
#endif//_WINDOWS
	}

	return ret;
}

void Thread::join(thread_t* t/* = nullptr*/)
{
	int ret{t ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		boost::thread* thread{reinterpret_cast<boost::thread*>(t)};
		thread->join();
	}
}
