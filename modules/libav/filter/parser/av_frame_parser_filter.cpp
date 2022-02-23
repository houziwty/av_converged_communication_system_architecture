#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_parser_filter.h"
using namespace module::av::stream;

AVFrameParserFilter::AVFrameParserFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), AVParserNode()
{}

AVFrameParserFilter::~AVFrameParserFilter()
{}

int AVFrameParserFilter::createNew(const AVModeConf& conf)
{
	//解析ID默认为2
	AVParserModeConf parserConf{2, AVParserType::AV_PARSER_TYPE_PS_PARSER};
	int ret{ AVParserNode::addConf(parserConf) };
	return Error_Code_Success == ret ? AVFilter::createNew(conf) : ret;
}

int AVFrameParserFilter::destroy()
{
	int ret{AVParserNode::removeConf(2)};
	return Error_Code_Success == ret ? AVFilter::destroy() : ret;
}

int AVFrameParserFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return AVParserNode::input(2, avpkt);
}

void AVFrameParserFilter::afterParsedDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		AVFilter::input(avpkt);
	}
}
