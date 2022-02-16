#include "av_data_parser_filter.h"
using namespace module::av::stream;

AVDataParserFilter::AVDataParserFilter() 
	: AVFilter()
{}

AVDataParserFilter::~AVDataParserFilter()
{}

int AVDataParserFilter::input(void* data/* = nullptr*/)
{
	return 0;
}
