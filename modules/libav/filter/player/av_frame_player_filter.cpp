#ifdef _WINDOWS

#include "libavpkt.h"
#include "error_code.h"
#include "pin/av_pin.h"
#include "av_frame_player_filter.h"
using namespace module::av::stream;

AVFramePlayerFilter::AVFramePlayerFilter(
	const AVFilterType type/* = AVFilterType::AV_FILTER_TYPE_NONE*/) 
	: AVFilter(type), Libavplayer(), flag{false}
{}

AVFramePlayerFilter::~AVFramePlayerFilter()
{}

int AVFramePlayerFilter::createNew(const AVModeConf& conf)
{
	//延迟加载播放器
	modeconf = conf;
	return AVFilter::createNew(conf);
}

int AVFramePlayerFilter::destroy(const uint32_t id /* = 0 */)
{
	int ret{ Libavplayer::removeConf(id) };
	return Error_Code_Success == ret ? AVFilter::destroy() : ret;
}

int AVFramePlayerFilter::input(
	const uint32_t id /* = 0 */, 
	const void* avpkt /* = nullptr */)
{
	int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		Libavpkt* pkt{reinterpret_cast<Libavpkt*>((void*)avpkt)};
		AVMainType maintype{pkt->maintype()};
		AVPlayerModeConf conf;

		if (!flag && AVMainType::AV_MAIN_TYPE_VIDEO == maintype)
		{
			conf.id = id;
			conf.type = AVPlayerType::AV_PLAYER_TYPE_D3D;
			conf.video.hwnd = modeconf.hwnd;
			conf.video.areas = modeconf.infos;
			flag = true;
			ret = Libavplayer::addConf(conf);
		}
		else if (!flag && AVMainType::AV_MAIN_TYPE_AUDIO == maintype)
		{
			conf.id = id;
			conf.type = AVPlayerType::AV_PLAYER_TYPE_AAC;
			flag = true;
			ret = Libavplayer::addConf(conf);
		}

		if (flag)
		{
			if (AVMainType::AV_MAIN_TYPE_VIDEO == maintype)
			{
				ret = Libavplayer::input(id, avpkt);
			}
			else if (AVMainType::AV_MAIN_TYPE_AUDIO == maintype)
			{
				ret = Libavplayer::input(id, avpkt);
			}
		}
	}
	
	return ret;
}

#endif//_WINDOWS
