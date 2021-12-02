#include "error_code.h"
#include "libmedia/pin.h"
#include "video_decoder_filter.h"
using namespace module::media::av;

VideoDecoderFilter::VideoDecoderFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

VideoDecoderFilter::~VideoDecoderFilter()
{}

int VideoDecoderFilter::createNew(void* param/* = nullptr*/)
{
	int ret{Filter::createNew(param)};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}
