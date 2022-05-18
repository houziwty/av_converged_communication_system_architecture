#include <ctime>
#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread/thread.hpp"
#include "time/xtime.h"
using namespace framework::utils::time;

XTime::XTime()
{
	memset(fmtgmt, 0, 64);
}

XTime::~XTime()
{}

const unsigned long long XTime::tickcount()
{
	boost::posix_time::ptime end{ 
		boost::posix_time::microsec_clock::local_time() };
	boost::posix_time::ptime begin{ 
		boost::posix_time::time_from_string("1970-01-01 00:00:00") };
	boost::posix_time::time_duration duration{ end - begin };

	return duration.total_milliseconds();
}

void XTime::sleep(const unsigned long long interval /* = 1 */)
{
	boost::this_thread::sleep(boost::posix_time::milliseconds(interval));
}

const char* XTime::gmt() const
{
    const std::time_t tt{std::time(nullptr)};
    std::strftime((char*)fmtgmt, 64, "%a, %b %d %Y %H:%M:%S GMT", std::gmtime(&tt));
    return fmtgmt;
}
