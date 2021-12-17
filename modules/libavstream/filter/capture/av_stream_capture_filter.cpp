#include "error_code.h"
#include "av_stream_capture_filter.h"
using namespace module::avstream;

AVStreamCaptureFilter::AVStreamCaptureFilter() 
	: AVFilter(FilterType::FILTER_TYPE_SOURCE)
{}

AVStreamCaptureFilter::~AVStreamCaptureFilter()
{}

int AVStreamCaptureFilter::createNew(void* param/* = nullptr*/)
{
	int ret{AVFilter::createNew()};

	if(Error_Code_Success == ret)
	{
		
	}

	return ret; 
}

int AVStreamCaptureFilter::input(void* data/* = nullptr*/)
{
	return 0;
}
