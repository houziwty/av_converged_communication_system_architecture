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
//

#ifndef MODULE_AV_REALPLAY_STREAM_RENDER_GRAPH_H
#define MODULE_AV_REALPLAY_STREAM_RENDER_GRAPH_H

#include "libavstream/av_stream_graph.h"

namespace module
{
	namespace avstream
	{
		class AV_STREAM_EXPORT RealplayStreamRenderGraph 
			: public AVGraph
		{
		public:
			RealplayStreamRenderGraph(void);
			virtual ~RealplayStreamRenderGraph(void);

		public:
			//创建
			//@param : 参数
			//@Return : 错误码
			//@Comment : 实时音视频流程图的构建内容包括 ：
			//						1. Video Decoder Filter
			//						2. Video Render Filter
			//						3. Audio Decoder Filter
			//						4. Audio Player Filter
			int createNew(void* param = nullptr) override;

		protected:
			int connectPin(void) override;
		};//class RealplayStreamRenderGraph
	}//namespace avstream
}//namespace module

#endif//MODULE_AV_REALPLAY_STREAM_RENDER_GRAPH_H
