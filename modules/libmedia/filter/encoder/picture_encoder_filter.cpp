#include "error_code.h"
#include "libmedia/pin.h"
#include "picture_encoder_filter.h"
using namespace module::media::av;

PictureEncoderFilter::PictureEncoderFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

PictureEncoderFilter::~PictureEncoderFilter()
{}

int PictureEncoderFilter::createNew()
{
	int ret{Filter::createNew()};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

int PictureEncoderFilter::input(FramePtr frame)
{
	return Error_Code_Success;
}

AVProcessorPtr PictureEncoderFilter::createNewProcessor()
{
	return 0;
}
