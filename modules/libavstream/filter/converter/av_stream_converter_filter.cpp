#include "av_stream_converter_filter.h"
using namespace module::avstream;

AVStreamConverterFilter::AVStreamConverterFilter() 
	: AVFilter()
{}

AVStreamConverterFilter::~AVStreamConverterFilter()
{}

int AVStreamConverterFilter::input(void* data/* = nullptr*/)
{
	return 0;
}
