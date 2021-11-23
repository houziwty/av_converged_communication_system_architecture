#include "error_code.h"
#include "libmedia/pin.h"
#include "video_render_filter.h"
using namespace module::media::av;

VideoRenderFilter::VideoRenderFilter() : MediaTargetFilter()
{}

VideoRenderFilter::~VideoRenderFilter()
{}

int VideoRenderFilter::createNew()
{
	int ret{MediaTargetFilter::createNew()};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

int VideoRenderFilter::input(FramePtr frame)
{
	return Error_Code_Success;
}

AVProcessorPtr VideoRenderFilter::createNewProcessor()
{
	return 0;
}
