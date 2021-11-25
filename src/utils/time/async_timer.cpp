#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/time/async_timer.h"
using namespace framework::utils::time;

AsyncTimer::AsyncTimer()
{}

AsyncTimer::~AsyncTimer()
{}

int AsyncTimer::setTimer(
	boost::asio::ip::tcp::socket& s, 
	AsyncExpireEventCallback callback, 
	const int expire /* = 5 */)
{
	int ret{0 < expire ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		auto self{
			boost::enable_shared_from_this<AsyncTimer>::shared_from_this()};
		using TimerPtr = boost::shared_ptr<boost::asio::deadline_timer>;
		TimerPtr ptr{
			boost::make_shared<boost::asio::deadline_timer>(
				s.get_executor(), 
				boost::posix_time::seconds(expire)) };

		if (ptr)
		{
			ptr->async_wait(
				[&](boost::system::error_code e)
				{
					if (callback)
					{
						callback(e.value());
					}
				});
		}
		else
		{
			ret = Error_Code_Bad_New_Object;
		}
	}

	return ret;
}
