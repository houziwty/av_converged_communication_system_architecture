#include "error_code.h"
#include "av_data_callback_filter.h"
using namespace module::media::av;

AVDataCallbackFilter::AVDataCallbackFilter() 
	: MediaTargetFilter()
{}

AVDataCallbackFilter::~AVDataCallbackFilter()
{}

int AVDataCallbackFilter::createNew()
{
	int ret{MediaTargetFilter::createNew()};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

int AVDataCallbackFilter::input(FramePtr frame)
{
	return Error_Code_Success;
}

AVProcessorPtr AVDataCallbackFilter::createNewProcessor()
{
	return 0;
}
