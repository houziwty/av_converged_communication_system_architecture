#include "av_stream_render_filter.h"
using namespace module::avstream;

AVStreamRenderFilter::AVStreamRenderFilter() 
	: AVFilter()
{}

AVStreamRenderFilter::~AVStreamRenderFilter()
{}

int AVStreamRenderFilter::input(void* data/* = nullptr*/)
{
	return 0;
}
