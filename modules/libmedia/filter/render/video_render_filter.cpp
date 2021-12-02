#include "error_code.h"
#include "libmedia/pin.h"
#include "video_render_filter.h"
using namespace module::media::av;

VideoRenderFilter::VideoRenderFilter() : MediaTargetFilter()
{}

VideoRenderFilter::~VideoRenderFilter()
{}

int VideoRenderFilter::createNew(void* param/* = nullptr*/)
{
	int ret{MediaTargetFilter::createNew(param)};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}
