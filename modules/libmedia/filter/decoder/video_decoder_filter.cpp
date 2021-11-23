#include "error_code.h"
#include "libmedia/pin.h"
#include "video_decoder_filter.h"
using namespace module::media::av;

VideoDecoderFilter::VideoDecoderFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

VideoDecoderFilter::~VideoDecoderFilter()
{}

int VideoDecoderFilter::createNew()
{
	int ret{Filter::createNew()};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

int VideoDecoderFilter::input(FramePtr frame)
{
	return Error_Code_Success;
}

AVProcessorPtr VideoDecoderFilter::createNewProcessor()
{
	return 0;
}