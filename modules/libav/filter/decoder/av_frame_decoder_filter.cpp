#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_decoder_filter.h"
using namespace module::av::stream;

AVFrameDecoderFilter::AVFrameDecoderFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), AVCodecNode()
{}

AVFrameDecoderFilter::~AVFrameDecoderFilter()
{}

int AVFrameDecoderFilter::createNew(const AVModeConf& conf)
{
	//解码ID默认为1
	AVCodecModeConf codecConf{1, AVCodecType::AV_CODEC_TYPE_DECODE_H264};
	int ret{ AVCodecNode::addConf(codecConf) };
	return Error_Code_Success == ret ? AVFilter::createNew(conf) : ret;
}

int AVFrameDecoderFilter::destroy()
{
	int ret{AVCodecNode::removeConf(1)};
	return Error_Code_Success == ret ? AVFilter::destroy() : ret;
}

int AVFrameDecoderFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return AVCodecNode::input(1, avpkt);
}

void AVFrameDecoderFilter::afterCodecDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		AVFilter::input(avpkt);
	}
}
