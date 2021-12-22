#ifdef _WINDOWS
#include <windows.h>
#include "boost/winapi/system.hpp"
#else
#include <sys/sysinfo.h>
#include <sched.h>
#include <pthread.h>
#endif//_WINDOWS
#include "utils/hardware/cpu.h"
using namespace framework::utils::hardware;

Cpu::Cpu()
{}

Cpu::~Cpu()
{}

const int Cpu::getCount() const
{
	int number{1};

#ifdef _WINDOWS
	boost::winapi::_SYSTEM_INFO info;
	boost::winapi::GetSystemInfo(&info);
	number = static_cast<int>(info.dwNumberOfProcessors);
#else
	number = static_cast<int>(get_nprocs());
#endif//_WINDOWS

	return number;
}
