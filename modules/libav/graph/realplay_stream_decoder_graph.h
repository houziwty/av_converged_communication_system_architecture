//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 实时流视频解码流程图类
//
//		History:
//					1. 2021-11-18 由王科威创建
//					2. 2022-02-175 只处理视频
//

#ifndef MODULE_AV_STREAM_REALPLAY_STREAM_DECODER_GRAPH_H
#define MODULE_AV_STREAM_REALPLAY_STREAM_DECODER_GRAPH_H

#include "av_graph.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class RealplayStreamDecoderGraph : public AVGraph
			{
			public:
				RealplayStreamDecoderGraph(void);
				virtual ~RealplayStreamDecoderGraph(void);

			public:
				//创建
				//@param : 参数
				//@Return : 错误码
				//@Comment : 实时视频分析流程图过滤器及顺序
				//			1. Buffer Parser Filter
				//			2. PS Parser Filter
				//			3. Video Decoder Filter
				//			4. YUV to BGR24 Filter
				//			5. Target Filter
				int createNew(void* param = nullptr) override;

			protected:
				int connectPin(void) override;
			};//class RealplayStreamDecoderGraph
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_REALPLAY_STREAM_DECODER_GRAPH_H
