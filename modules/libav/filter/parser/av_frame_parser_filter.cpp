#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_parser_filter.h"
using namespace module::av::stream;

AVFrameParserFilter::AVFrameParserFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), AVParserNode(), offset{10000}
{}

AVFrameParserFilter::~AVFrameParserFilter()
{}

int AVFrameParserFilter::createNew(const AVModeConf& conf)
{
	int ret{ 0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		AVParserModeConf parserConf{ conf.id + offset, AVParserType::AV_PARSER_TYPE_PS_PARSER };
		ret = AVParserNode::addConf(parserConf);

		if (Error_Code_Success == ret)
		{
			ret = AVFilter::createNew(conf);
		}
	}
	
	return ret;
}

int AVFrameParserFilter::destroy(const uint32_t id /* = 0 */)
{
	int ret{AVParserNode::removeConf(id + offset)};
	return Error_Code_Success == ret ? AVFilter::destroy() : ret;
}

int AVFrameParserFilter::input(
	const uint32_t id /* = 0 */, 
	const AVPkt* avpkt /* = nullptr */)
{
	return AVParserNode::input(id + offset, avpkt);
}

void AVFrameParserFilter::afterParsedDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		//数据传递要减去偏移量
		AVFilter::input(id - offset, avpkt);
	}
}
