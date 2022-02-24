//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-19
//		Description : AV流播放过滤器
//
//		History:
//					1. 2021-11-19 由王科威创建
//

#ifndef MODULE_AV_STREAM_FRAME_PLAYER_FILTER_H
#define MODULE_AV_STREAM_FRAME_PLAYER_FILTER_H

#include "av_player_node.h"
#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVFramePlayerFilter 
				: public AVFilter, protected AVPlayerNode
			{
			public:
				AVFramePlayerFilter(
					const AVFilterType type = AVFilterType::AV_FILTER_TYPE_NONE);
				virtual ~AVFramePlayerFilter(void);

			public:
				int createNew(const AVModeConf& conf) override;
				int destroy(void) override;
				int input(const AVPkt* avpkt = nullptr) override;

			private:
				bool flag;
				AVModeConf modeconf;
			};//class AVFramePlayerFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_FRAME_PLAY_FILTER_H