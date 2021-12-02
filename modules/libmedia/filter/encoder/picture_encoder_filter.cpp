#include "error_code.h"
#include "libmedia/pin.h"
#include "picture_encoder_filter.h"
using namespace module::media::av;

PictureEncoderFilter::PictureEncoderFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

PictureEncoderFilter::~PictureEncoderFilter()
{}

int PictureEncoderFilter::createNew(void* param/* = nullptr*/)
{
	int ret{Filter::createNew(param)};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}
