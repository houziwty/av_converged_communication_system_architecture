#include "av_stream_encoder_filter.h"
using namespace module::avstream;

AVStreamEncoderFilter::AVStreamEncoderFilter() 
	: AVFilter()
{}

AVStreamEncoderFilter::~AVStreamEncoderFilter()
{}

int AVStreamEncoderFilter::input(void* data/* = nullptr*/)
{
	return 0;
}
