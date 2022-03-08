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
	int ret{ 0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		AVParserModeConf parserConf{ conf.id, AVParserType::AV_PARSER_TYPE_BUFFER_PARSER };
		ret = AVParserNode::addConf(parserConf);

		if (Error_Code_Success == ret)
		{
			ret = AVFilter::createNew(conf);
		}
	}
	
	return ret;
}

int AVBufferParserFilter::destroy(const uint32_t id /* = 0 */)
{
	int ret{AVParserNode::removeConf(id)};
	return Error_Code_Success == ret ? AVFilter::destroy() : ret;
}

int AVBufferParserFilter::input(
	const uint32_t id /* = 0 */, 
	const AVPkt* avpkt /* = nullptr */)
{
	return AVParserNode::input(id, avpkt);
}

void AVBufferParserFilter::afterParsedDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		AVFilter::input(id, avpkt);
	}
}
