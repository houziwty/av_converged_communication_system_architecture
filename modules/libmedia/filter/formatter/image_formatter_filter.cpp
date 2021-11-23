#include "error_code.h"
#include "libmedia/pin.h"
#include "image_formatter_filter.h"
using namespace module::media::av;

ImageFormatterFilter::ImageFormatterFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

ImageFormatterFilter::~ImageFormatterFilter()
{}

int ImageFormatterFilter::createNew()
{
	int ret{Filter::createNew()};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

int ImageFormatterFilter::input(FramePtr frame)
{
	return Error_Code_Success;
}

AVProcessorPtr ImageFormatterFilter::createNewProcessor()
{
	return 0;
}
