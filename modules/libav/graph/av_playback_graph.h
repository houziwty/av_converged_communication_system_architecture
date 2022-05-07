//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-07
//		Description : 音视频历史播放流程图类
//
//		History:
//					1. 2022-05-07 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PLAYBACK_GRAPH_H
#define MODULE_AV_STREAM_AV_PLAYBACK_GRAPH_H

#include "av_graph.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVPlaybackGraph : public AVGraph
			{
			public:
				AVPlaybackGraph(void);
				virtual ~AVPlaybackGraph(void);

			public:
				//创建
				//@conf [in] : 配置参数
				//@Return : 错误码
				//@Comment : 固定流程
				//			1. Buffer Parser Filter
				//			2. PS Parser Filter
				//			3. Frame Decoder Filter
				//			4. Frame Player Filter
				int createNew(const AVModeConf& conf) override;

			protected:
				int connectPin(void) override;
			};//class AVPlaybackGraph
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PLAYBACK_GRAPH_H
