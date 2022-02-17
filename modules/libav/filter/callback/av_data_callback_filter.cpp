#include "av_data_callback_filter.h"
using namespace module::av::stream;

AVDataCallbackFilter::AVDataCallbackFilter() 
	: AVFilter(AVFilterType::AV_FILTER_TYPE_TARGET)
{}

AVDataCallbackFilter::~AVDataCallbackFilter()
{}

int AVDataCallbackFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	// int ret{data ? Error_Code_Success : Error_Code_Invalid_Param};

	// if(Error_Code_Success == ret && avstreamResultCallback)
	// {
	// 	avstreamResultCallback(data);
	// }

	// return ret;

	return 0;
}

// void AVStreamCallbackFilter::setAVStreamCallback(AVStreamResultCallback callback)
// {
// 	avstreamResultCallback = callback;
// }
