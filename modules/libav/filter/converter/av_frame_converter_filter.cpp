#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_converter_filter.h"
using namespace module::av::stream;

AVFrameConverterFilter::AVFrameConverterFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), AVCodecNode()
{}

AVFrameConverterFilter::~AVFrameConverterFilter()
{}

int AVFrameConverterFilter::createNew(const AVModeConf& conf)
{
	//转换器ID默认2
	AVCodecModeConf codecConf{ 2, AVCodecType::AV_CODEC_TYPE_YUV420P_2_BGR24 };
	int ret{ AVCodecNode::addConf(codecConf) };
	return Error_Code_Success == ret ? AVFilter::createNew(conf) : ret;
}

int AVFrameConverterFilter::destroy()
{
	int ret{AVCodecNode::removeConf(2)};
	return Error_Code_Success == ret ? AVFilter::destroy() : ret;
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
		AVFilter::input(avpkt);
	}
}
