#include "av_pkt.h"
#include "error_code.h"
#include "pin/av_pin.h"
#include "av_ps_parser_filter.h"
using namespace module::av::stream;

AVPSParserFilter::AVPSParserFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type, AVFilterConf::AV_FILTER_CONF_AV), AVParserNode()
{}

AVPSParserFilter::~AVPSParserFilter()
{}

int AVPSParserFilter::createNew(const AVModeConf& conf)
{
	//解析ID默认为2
	AVParserModeConf parserConf{2, AVParserModeConf::AV_PARSER_TYPE_PS_PARSER};
	int ret{ AVParserNode::addConf(parserConf) };
	return Error_Code_Success == ret ? AVFilter::createNew(&conf) : ret;
}

int AVPSParserFilter::destroy()
{
	int ret{AVFilter::destroy()};
	return Error_Code_Success == ret ? AVParserNode::removeConf(2) : ret;
}

int AVPSParserFilter::input(
	const AVPkt* avpkt/* = nullptr*/)
{
	return AVParserNode::input(2, avpkt);
}

void AVPSParserFilter::afterParsedDataNotification(
	const uint32_t id/* = 0*/,  
	const AVPkt* avpkt/* = nullptr*/)
{
	if (0 < id && avpkt)
	{
		const AVSubType subtype{avpkt->subtype()};

		//Video only
		if (AVSubType::AV_SUB_TYPE_VIDEO == subtype || 
			AVSubType::AV_SUB_TYPE_IDR == subtype)
		{
			AVPinRef pin{AVFilter::query(av_video_output_pin_name)};
			if (!pin.expired())
			{
				pin.lock()->input(avpkt);
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
