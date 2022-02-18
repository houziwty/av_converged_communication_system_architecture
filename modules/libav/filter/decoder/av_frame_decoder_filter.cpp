#include "error_code.h"
#include "av_pkt.h"
#include "pin/av_pin.h"
#include "av_frame_decoder_filter.h"
using namespace module::av::stream;

AVFrameDecoderFilter::AVFrameDecoderFilter() 
	: AVFilter(AVFilterType::AV_FILTER_TYPE_MEDIUM), AVCodecNode()
{}

AVFrameDecoderFilter::~AVFrameDecoderFilter()
{}

int AVFrameDecoderFilter::createNew(void* param/* = nullptr*/)
{
	AVCodecModeConf conf{1, AVCodecType::AV_CODEC_TYPE_DECODE_H264};
	int ret{ AVCodecNode::addConf(conf) };

	if (Error_Code_Success == ret)
	{
		AVFilterConf conf{1, nullptr};
		ret = AVFilter::createNew(&conf);
	}
	
	return ret;
}

int AVFrameDecoderFilter::destroy()
{
	int ret{AVFilter::destroy()};
	return Error_Code_Success == ret ? AVCodecNode::removeConf(1) : ret;
}

int AVFrameDecoderFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return AVCodecNode::input(1, avpkt);
}

void AVFrameDecoderFilter::afterCodecDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{

}
