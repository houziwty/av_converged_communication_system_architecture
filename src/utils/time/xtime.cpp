#include "boost/date_time/posix_time/posix_time.hpp"
#include "boost/thread/thread.hpp"
#include "utils/time/xtime.h"
using namespace framework::utils::time;

XTime::XTime()
{}

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
