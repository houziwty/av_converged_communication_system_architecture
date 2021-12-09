#include "av_stream_decoder_filter.h"
using namespace module::avstream;

AVStreamDecoderFilter::AVStreamDecoderFilter() 
	: AVFilter()
{}

AVStreamDecoderFilter::~AVStreamDecoderFilter()
{}

int AVStreamDecoderFilter::input(void* data/* = nullptr*/)
{
	return 0;
}
