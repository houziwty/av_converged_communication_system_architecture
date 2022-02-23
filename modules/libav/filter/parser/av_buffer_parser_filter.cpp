#include "error_code.h"
#include "pin/av_pin.h"
#include "av_buffer_parser_filter.h"
using namespace module::av::stream;

AVBufferParserFilter::AVBufferParserFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), AVParserNode()
{}

AVBufferParserFilter::~AVBufferParserFilter()
{}

int AVBufferParserFilter::createNew(const AVModeConf& conf)
{
	//解析ID默认为1
	AVParserModeConf parserConf{1, AVParserType::AV_PARSER_TYPE_BUFFER_PARSER};
	int ret{ AVParserNode::addConf(parserConf) };
	return Error_Code_Success == ret ? AVFilter::createNew(conf) : ret;
}

int AVBufferParserFilter::destroy()
{
	int ret{AVParserNode::removeConf(1)};
	return Error_Code_Success == ret ? AVFilter::destroy() : ret;
}

int AVBufferParserFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return AVParserNode::input(1, avpkt);
}

void AVBufferParserFilter::afterParsedDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		AVFilter::input(avpkt);
	}
}
