#include "av_frame_render_filter.h"
using namespace module::av::stream;

AVFrameRenderFilter::AVFrameRenderFilter() 
	: AVFilter(AVFilterType::AV_FILTER_TYPE_TARGET)
{}

AVFrameRenderFilter::~AVFrameRenderFilter()
{}

int AVFrameRenderFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return 0;
}
