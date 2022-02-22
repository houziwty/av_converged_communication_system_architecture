#include "av_pkt.h"
#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_converter_filter.h"
using namespace module::av::stream;

AVFrameConverterFilter::AVFrameConverterFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type, AVFilterConf::AV_FILTER_CONF_VIDEO), AVCodecNode()
{}

AVFrameConverterFilter::~AVFrameConverterFilter()
{}

int AVFrameConverterFilter::createNew(const AVModeConf& conf)
{
	//转换器默认2
	AVCodecModeConf codecConf{ 2, AVCodecType::AV_CODEC_TYPE_PICTURE_CONVERT };
	int ret{ AVCodecNode::addConf(codecConf) };
	return Error_Code_Success == ret ? AVFilter::createNew(conf) : ret;
}

int AVFrameConverterFilter::destroy()
{
	int ret{ AVFilter::destroy() };
	return Error_Code_Success == ret ? AVCodecNode::removeConf(2) : ret;
}

int AVFrameConverterFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return AVCodecNode::input(2, avpkt);
}

void AVFrameConverterFilter::afterCodecDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		const AVSubType subtype{avpkt->subtype()};

		if (AVSubType::AV_SUB_TYPE_YUV420P == subtype || 
			AVSubType::AV_SUB_TYPE_RGB24 == subtype || 
			AVSubType::AV_SUB_TYPE_BGR24 == subtype)
		{
			AVPinPtr video_out_pin{avpins.at(av_video_output_pin_name)};
			if (video_out_pin)
			{
				video_out_pin->input(avpkt);
			}
		}
		// else if(AVSubType::AV_SUB_TYPE_AUDIO == subtype || 
		// 		AVSubType::AV_SUB_TYPE_G711 == subtype || 
		// 		AVSubType::AV_SUB_TYPE_G722 == subtype || 
		// 		AVSubType::AV_SUB_TYPE_ADPCM == subtype)
		// {
		// 	AVPinPtr audio_out_pin{avpins.at(av_audio_output_pin_name)};
		// 	if (audio_out_pin)
		// 	{
		// 		audio_out_pin->input(avpkt);
		// 	}
		// }
	}
}
