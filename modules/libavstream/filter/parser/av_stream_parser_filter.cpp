#include "av_stream_parser_filter.h"
using namespace module::avstream;

AVStreamParserFilter::AVStreamParserFilter() 
	: AVFilter()
{}

AVStreamParserFilter::~AVStreamParserFilter()
{}

int AVStreamParserFilter::input(void* data/* = nullptr*/)
{
	return 0;
}
