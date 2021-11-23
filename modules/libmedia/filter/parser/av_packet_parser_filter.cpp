#include "error_code.h"
#include "av_packet_parser_filter.h"
using namespace module::media::av;

AVPacketParserFilter::AVPacketParserFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

AVPacketParserFilter::~AVPacketParserFilter()
{}

int AVPacketParserFilter::createNew()
{
	int ret{Filter::createNew()};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

int AVPacketParserFilter::input(FramePtr frame)
{
	return Error_Code_Success;
}

AVProcessorPtr AVPacketParserFilter::createNewProcessor()
{
	return 0;
}
