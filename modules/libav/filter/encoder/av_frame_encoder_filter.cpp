#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_encoder_filter.h"
using namespace module::av::stream;

AVFrameEncoderFilter::AVFrameEncoderFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), AVCodecNode(), offset{ 30000 }
{}

AVFrameEncoderFilter::~AVFrameEncoderFilter()
{}

int AVFrameEncoderFilter::createNew(const AVModeConf& conf)
{
	int ret{ 0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		AVCodecType type{AVCodecType::AV_CODEC_TYPE_NONE};
		if (AVModeType::AV_MODE_TYPE_GRAB_JPEG == conf.type)
		{
			type = AVCodecType::AV_CODEC_TYPE_ENCODE_JPEG;
		}

		AVCodecModeConf codecConf{ conf.id + offset, type };
		ret = AVCodecNode::addConf(codecConf);

		if (Error_Code_Success == ret)
		{
			ret = AVFilter::createNew(conf);
		}
	}
	
	return ret;
}

int AVFrameEncoderFilter::destroy(const uint32_t id /* = 0 */)
{
	int ret{AVCodecNode::removeConf(id + offset)};
	return Error_Code_Success == ret ? AVFilter::destroy() : ret;
}

int AVFrameEncoderFilter::input(
	const uint32_t id /* = 0 */, 
	const AVPkt* avpkt /* = nullptr */)
{
	return AVCodecNode::input(id + offset, avpkt);
}

void AVFrameEncoderFilter::afterCodecDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		AVFilter::input(id - offset, avpkt);
	}
}
