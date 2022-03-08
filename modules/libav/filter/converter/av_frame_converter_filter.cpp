#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_converter_filter.h"
using namespace module::av::stream;

AVFrameConverterFilter::AVFrameConverterFilter(
	const AVFilterType type /* = AVFilterType::AV_FILTER_TYPE_NONE */)
	: AVFilter(type), AVCodecNode(), offset{ 10000 }
{}

AVFrameConverterFilter::~AVFrameConverterFilter()
{}

int AVFrameConverterFilter::createNew(const AVModeConf& conf)
{
	int ret{ 0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		AVCodecType codec{ AVCodecType::AV_CODEC_TYPE_NONE };
		if (AVModeType::AV_MODE_TYPE_GRAB_BRG24 == conf.type)
		{
			codec = AVCodecType::AV_CODEC_TYPE_YUV420P_2_BGR24;
		}
		AVCodecModeConf codecConf{ conf.id + offset, codec };
		ret = AVCodecNode::addConf(codecConf);

		if (Error_Code_Success == ret)
		{
			ret = AVFilter::createNew(conf);
		}
	}
	
	return ret;
}

int AVFrameConverterFilter::destroy(const uint32_t id /* = 0 */)
{
	int ret{AVCodecNode::removeConf(id + offset)};
	return Error_Code_Success == ret ? AVFilter::destroy() : ret;
}

int AVFrameConverterFilter::input(
	const uint32_t id /* = 0 */, 
	const AVPkt* avpkt /* = nullptr */)
{
	return AVCodecNode::input(id + offset, avpkt);
}

void AVFrameConverterFilter::afterCodecDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		//数据传递要减去偏移量
		AVFilter::input(id - offset, avpkt);
	}
}
