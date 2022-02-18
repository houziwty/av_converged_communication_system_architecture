#include "error_code.h"
#include "av_pkt.h"
#include "pin/av_pin.h"
#include "av_pkt_parser_filter.h"
using namespace module::av::stream;

AVPktParserFilter::AVPktParserFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), AVParserNode()
{}

AVPktParserFilter::~AVPktParserFilter()
{}

int AVPktParserFilter::createNew(void* param/* = nullptr*/)
{
	conf = *reinterpret_cast<AVParserModeConf*>(param);
	int ret{ AVParserNode::addConf(conf) };

	if (Error_Code_Success == ret)
	{
		AVFilterConf conf{1, nullptr};
		ret = AVFilter::createNew(&conf);
	}
	
	return ret;
}

int AVPktParserFilter::destroy()
{
	int ret{AVFilter::destroy()};
	return Error_Code_Success == ret ? AVParserNode::removeConf(conf.id) : ret;
}

int AVPktParserFilter::input(
	const AVPkt* avpkt/* = nullptr*/)
{
	return AVParserNode::input(conf.id, avpkt);
}

void AVPktParserFilter::afterParsedDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		const AVSubType subtype{avpkt->subtype()};

		if (AVSubType::AV_SUB_TYPE_VIDEO == subtype || 
			AVSubType::AV_SUB_TYPE_IDR == subtype || 
			AVSubType::AV_SUB_TYPE_B == subtype || 
			AVSubType::AV_SUB_TYPE_P == subtype)
		{
			AVPinPtr video_out_pin{avpins.at(av_video_output_pin_name)};
			if (video_out_pin)
			{
				video_out_pin->input(avpkt);
			}
		}
		else if(AVSubType::AV_SUB_TYPE_AUDIO == subtype || 
				AVSubType::AV_SUB_TYPE_G711 == subtype || 
				AVSubType::AV_SUB_TYPE_G722 == subtype || 
				AVSubType::AV_SUB_TYPE_ADPCM == subtype)
		{
			AVPinPtr audio_out_pin{avpins.at(av_audio_output_pin_name)};
			if (audio_out_pin)
			{
				audio_out_pin->input(avpkt);
			}
		}
	}
}
