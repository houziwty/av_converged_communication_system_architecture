#include "av_frame_converter_filter.h"
using namespace module::av::stream;

AVFrameConverterFilter::AVFrameConverterFilter() 
	: AVFilter(AVFilterType::AV_FILTER_TYPE_MEDIUM)
{}

AVFrameConverterFilter::~AVFrameConverterFilter()
{}

int AVFrameConverterFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return 0;
}
