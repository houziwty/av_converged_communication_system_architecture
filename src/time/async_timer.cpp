#include "boost/make_shared.hpp"
#include "error_code.h"
#include "time/async_timer.h"
using namespace framework::utils::time;

AsyncTimer::AsyncTimer(AsyncExpireEventCallback callback) 
	: asyncExpireEventCallback{callback}
{}

AsyncTimer::~AsyncTimer()
{}

int AsyncTimer::run(
	boost::asio::io_context* ctx/* = nullptr*/, 
	const int expire /* = 5000 */)
{
	int ret{ctx && 0 < expire ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		auto self{
			boost::enable_shared_from_this<AsyncTimer>::shared_from_this()};
		using TimerPtr = boost::shared_ptr<boost::asio::deadline_timer>;
		TimerPtr ptr{
			boost::make_shared<boost::asio::deadline_timer>(
				*ctx, 
				boost::posix_time::seconds(expire)) };

		if (ptr)
		{
			ptr->async_wait(
				[&, self](boost::system::error_code e)
				{
					if (asyncExpireEventCallback)
					{
						asyncExpireEventCallback(e.value());
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
