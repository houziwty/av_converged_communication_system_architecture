#include "video_frame_analysis_filter.h"
using namespace module::av::stream;

VideoFrameAnalysisFilter::VideoFrameAnalysisFilter() 
	: AVFilter(AVFilterType::AV_FILTER_TYPE_MEDIUM)
{}

VideoFrameAnalysisFilter::~VideoFrameAnalysisFilter()
{}

int VideoFrameAnalysisFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return 0;
}
