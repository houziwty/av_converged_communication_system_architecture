#include "av_pkt.h"
#include "av_data_parser_filter.h"
using namespace module::av::stream;

AVDataParserFilter::AVDataParserFilter() 
	: AVFilter(), AVParserNode()
{}

AVDataParserFilter::~AVDataParserFilter()
{}

int AVDataParserFilter::createNew(void* /*param = nullptr*/)
{
	//Default 1
	AVParserModeConf conf{1, AVParserType::AV_PARSER_TYPE_BUFFER_PARSER};
	int ret{AVParserNode::addConf(conf)};
	return Error_Code_Success == ret ? AVFilter::createNew() : ret;
}

int AVDataParserFilter::destroy()
{
	int ret{AVFilter::destroy()};
	return Error_Code_Success == ret ? AVParserNode::removeConf(1) : ret;
}

int AVDataParserFilter::input(
	const AVPkt* avpkt/* = nullptr*/)
{
	return AVParserNode::input(1, data, bytes);
}

void AVDataParserFilter::afterParsedDataNotification(
	const uint32_t id/* = 0*/, 
	const MainType maintype/* = MainType::MAIN_TYPE_NONE*/, 
	const SubType subtype/* = SubType::SUB_TYPE_NONE*/, 
	const uint32_t bytes/* = 0*/, 
	const uint64_t seqence/* = 0*/, 
	const uint64_t timestamp/* = 0*/, 
	const void* data/* = nullptr*/)
{
	
}
