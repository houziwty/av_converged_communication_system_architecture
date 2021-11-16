#ifdef WINDOWS
#include <windows.h>
#include "boost/winapi/system.hpp"
#else
#include <sys/sysinfo.h>
#include <sched.h>
#include <pthread.h>
#endif//WINDOWS
#ifdef OS_WINDOWS
#include "boost/winapi/system.hpp"
#else
#include <sys/sysinfo.h>
#endif//OS_WINDOWS
#include "utils/hw/cpu.h"
using namespace utils::hw;

Cpu::Cpu()
{}

Cpu::~Cpu()
{}

const size_t Cpu::getCount() const
{
	size_t number = 1;

#ifdef WINDOWS
	boost::winapi::_SYSTEM_INFO info;
	boost::winapi::GetSystemInfo(&info);
	number = info.dwNumberOfProcessors;
#else
	number = get_nprocs();
#endif//WINDOWS

	return tempNumberOfCores;
}A
