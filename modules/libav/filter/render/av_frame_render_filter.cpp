#include "error_code.h"
#include "av_frame_render_filter.h"
using namespace module::av::stream;

AVFrameRenderFilter::AVFrameRenderFilter() 
	: AVFilter(AVFilterType::AV_FILTER_TYPE_TARGET), VideoRenderNode()
{}

AVFrameRenderFilter::~AVFrameRenderFilter()
{}

int AVFrameRenderFilter::createNew(void* hwnd/* = nullptr*/)
{
	VideoRenderModeConf conf{ 1, hwnd };
	int ret{ VideoRenderNode::addConf(conf) };

	if (Error_Code_Success == ret)
	{
		AVFilterConf conf{ 1, hwnd };
		ret = AVFilter::createNew(&conf);
	}

	return ret;
}

int AVFrameRenderFilter::destroy()
{
	int ret{ AVFilter::destroy() };
	return Error_Code_Success == ret ? VideoRenderNode::removeConf(1) : ret;
}

int AVFrameRenderFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return VideoRenderNode::input(1, avpkt);
}
