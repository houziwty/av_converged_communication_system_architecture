//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 实时音视频播放流程图类
//
//		History:
//					1. 2021-11-18 由王科威创建
//					2. 2022-02-15 只处理视频
//

#ifndef MODULE_AV_STREAM_REALPLAY_STREAM_RENDER_GRAPH_H
#define MODULE_AV_STREAM_REALPLAY_STREAM_RENDER_GRAPH_H

#include "av_graph.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class RealplayStreamRenderGraph : public AVGraph
			{
			public:
				RealplayStreamRenderGraph(void);
				virtual ~RealplayStreamRenderGraph(void);

			public:
				//创建
				//@hwnd : 播放窗口句柄
				//@Return : 错误码
				//@Comment : 实时视频流程图过滤器及顺序
				//			1. Data Parser Filter
				//			2. Video Decoder Filter
				//			3. Video Render Filter
				int createNew(void* hwnd = nullptr) override;

			protected:
				int connectPin(void) override;
			};//class RealplayStreamRenderGraph
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_REALPLAY_STREAM_RENDER_GRAPH_H
