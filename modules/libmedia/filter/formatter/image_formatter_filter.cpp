#include "error_code.h"
#include "libmedia/pin.h"
#include "image_formatter_filter.h"
using namespace module::media::av;

ImageFormatterFilter::ImageFormatterFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

ImageFormatterFilter::~ImageFormatterFilter()
{}

int ImageFormatterFilter::createNew(void* param/* = nullptr*/)
{
	int ret{Filter::createNew(param)};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}
