#include "error_code.h"
#include "av_stream_callback_filter.h"
using namespace module::avstream;

AVStreamCallbackFilter::AVStreamCallbackFilter() 
	: AVFilter(FilterType::FILTER_TYPE_TARGET)
{}

AVStreamCallbackFilter::~AVStreamCallbackFilter()
{}

int AVStreamCallbackFilter::input(void* data/* = nullptr*/)
{
	int ret{data ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret && avstreamResultCallback)
	{
		avstreamResultCallback(data);
	}

	return ret;
}

void AVStreamCallbackFilter::setAVStreamCallback(AVStreamResultCallback callback)
{
	avstreamResultCallback = callback;
}
