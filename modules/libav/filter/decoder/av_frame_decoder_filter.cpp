#include "av_frame_decoder_filter.h"
using namespace module::av::stream;

AVFrameDecoderFilter::AVFrameDecoderFilter() 
	: AVFilter(AVFilterType::AV_FILTER_TYPE_MEDIUM)
{}

AVFrameDecoderFilter::~AVFrameDecoderFilter()
{}

int AVFrameDecoderFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return 0;
}
