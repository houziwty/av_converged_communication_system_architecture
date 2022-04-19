#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_decoder_filter.h"
using namespace module::av::stream;

AVFrameDecoderFilter::AVFrameDecoderFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), Libavcodec()
{}

AVFrameDecoderFilter::~AVFrameDecoderFilter()
{}

int AVFrameDecoderFilter::createNew(const AVModeConf& conf)
{
	int ret{ 0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		AVCodecModeConf codecConf{ conf.id, AVCodecType::AV_CODEC_TYPE_DECODE_H264 };
		ret = Libavcodec::addConf(codecConf);

		if (Error_Code_Success == ret)
		{
			ret = AVFilter::createNew(conf);
		}
	}
	
	return ret;
}

int AVFrameDecoderFilter::destroy(const uint32_t id /* = 0 */)
{
	int ret{Libavcodec::removeConf(id)};
	return Error_Code_Success == ret ? AVFilter::destroy() : ret;
}

int AVFrameDecoderFilter::input(
	const uint32_t id /* = 0 */, 
	const void* avpkt /* = nullptr */)
{
	return Libavcodec::input(id, avpkt);
}

void AVFrameDecoderFilter::afterCodecDataNotification(
	const uint32_t id/* = 0*/,  
	const void* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		AVFilter::input(id, avpkt);
	}
}
