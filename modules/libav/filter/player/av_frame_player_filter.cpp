#ifdef _WINDOWS
#include "av_pkt.h"
#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_player_filter.h"
using namespace module::av::stream;

AVFramePlayerFilter::AVFramePlayerFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), AVPlayerNode(), flag{false}
{}

AVFramePlayerFilter::~AVFramePlayerFilter()
{}

int AVFramePlayerFilter::createNew(const AVModeConf& conf)
{
	//延迟加载播放器
	modeconf = conf;
	return AVFilter::createNew(conf);
}

int AVFramePlayerFilter::destroy()
{
	int ret{AVFilter::destroy()};

	if (Error_Code_Success == ret)
	{
		AVPlayerNode::removeConf(1);
		AVPlayerNode::removeConf(2);
	}
	
	return ret;
}

int AVFramePlayerFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVMainType maintype{avpkt->maintype()};
		AVPlayerModeConf conf;

		if (!flag && AVMainType::AV_MAIN_TYPE_VIDEO == maintype)
		{
			conf.id = 1;
			conf.type = AVPlayerType::AV_PLAYER_TYPE_D3D;
			conf.video.hwnd = modeconf.hwnd;
			conf.video.areas = modeconf.areas;
			flag = true;
			ret = AVPlayerNode::addConf(conf);
		}
		else if (!flag && AVMainType::AV_MAIN_TYPE_AUDIO == maintype)
		{
			conf.id = 2;
			conf.type = AVPlayerType::AV_PLAYER_TYPE_AAC;
			flag = true;
			ret = AVPlayerNode::addConf(conf);
		}

		if (flag)
		{
			if (AVMainType::AV_MAIN_TYPE_VIDEO == maintype)
			{
				ret = AVPlayerNode::input(1, avpkt);
			}
			else if (AVMainType::AV_MAIN_TYPE_AUDIO == maintype)
			{
				ret = AVPlayerNode::input(2, avpkt);
			}
		}
	}
	
	return ret;
}

#endif//_WINDOWS
