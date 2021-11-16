#ifdef OS_WINDOWS
#include "boost/winapi/system.hpp"
#else
#include <pthread.h>
#endif//OS_WINDOWS
#include "error_code.h"
#include "utils/thread/thread.h"
using namespace utils::thread;

Thread::Thread()
{}

Thread::~Thread()
{}

int Thread::setAffinity(const int idx /* = 0 */)
{
	int ret{Error_Code_Success};

#ifdef OS_WINDOWS
	if(-1 < idx)
	{
		DWORD mask{0x01 << idx};
		ret = (!SetThreadAffinityMask(GetCurrentThread(), &mask) ? Error_Code_Operate_Failure : Error_Code_Success);
	}
#else
	cpu_set_t param;
	CPU_ZERO(&param);
	if (-1 < idx)
	{
		CPU_SET(idx, &param);
	}
	ret = (!pthread_setaffinity_np(pthread_self(), sizeof(param), &param) ? Error_Code_Success : Error_Code_Operate_Failure);
#endif//OS_WINDOWS

	return ret;
}
