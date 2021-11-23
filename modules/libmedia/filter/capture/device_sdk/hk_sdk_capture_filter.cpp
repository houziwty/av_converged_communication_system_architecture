#include "error_code.h"
#include "hk_sdk_capture_filter.h"
using namespace module::media::av;

HKSdkCaptureFilter::HKSdkCaptureFilter() 
	: MediaSourceFilter()
{}

HKSdkCaptureFilter::~HKSdkCaptureFilter()
{}

int HKSdkCaptureFilter::createNew()
{
	int ret{MediaSourceFilter::createNew()};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

AVProcessorPtr HKSdkCaptureFilter::createNewProcessor()
{
	return 0;
}
