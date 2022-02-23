#include "av_pkt.h"
#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_player_filter.h"
using namespace module::av::stream;

AVFramePlayFilter::AVFramePlayFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), AVPlayerNode(), vid{0}, aid{0}, hwnd{nullptr}
{}

AVFramePlayFilter::~AVFramePlayFilter()
{}

int AVFramePlayFilter::createNew(const AVModeConf& conf)
{
	//延迟加载播放器
	modeconf = conf;
	return AVFilter::createNew(conf);
}

int AVFramePlayFilter::destroy()
{
	int ret{AVFilter::destroy()};

	if (Error_Code_Success == ret)
	{
		AVPlayerNode::removeConf(vid);
		AVPlayerNode::removeConf(aid);
	}
	
	return ret;
}

int AVFramePlayFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVMainType maintype{avpkt->maintype()};
		AVPlayerModeConf conf;

		if (!vid && AVMainType::AV_MAIN_TYPE_VIDEO == maintype)
		{
			conf.id = 1;
			conf.type = AVPlayerType::AV_PLAYER_TYPE_D3D;
			conf.video.hwnd = modeconf.hwnd;
		}
		else if (!aid && AVMainType::AV_MAIN_TYPE_AUDIO == maintype)
		{
			conf.id = 2;
			conf.type = AVPlayerType::AV_PLAYER_TYPE_AAC;
		}

		ret = AVPlayerNode::addConf(conf);
	}

	if (Error_Code_Success == ret)
	{
		if (AVMainType::AV_MAIN_TYPE_VIDEO == maintype)
		{
			ret = AVPlayerNode::input(1, avpkt);
		}
		else if (!aid && AVMainType::AV_MAIN_TYPE_AUDIO == maintype)
		{
			ret = AVPlayerNode::input(2, avpkt);
		}
	}
	
	return ret;
}
