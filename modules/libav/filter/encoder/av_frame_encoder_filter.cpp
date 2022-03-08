#include "av_frame_encoder_filter.h"
using namespace module::av::stream;

AVFrameEncoderFilter::AVFrameEncoderFilter() 
	: AVFilter(AVFilterType::AV_FILTER_TYPE_MEDIUM)
{}

AVFrameEncoderFilter::~AVFrameEncoderFilter()
{}

int AVFrameEncoderFilter::input(
	const uint32_t id /* = 0 */, 
	const AVPkt* avpkt /* = nullptr */)
{
	return 0;
}
