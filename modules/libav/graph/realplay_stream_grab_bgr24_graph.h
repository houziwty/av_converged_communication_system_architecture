//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 实时流BGR24数据抓取流程图
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_AV_STREAM_GRAB_BGR24_GRAPH_H
#define MODULE_AV_STREAM_GRAB_BGR24_GRAPH_H

#include "av_graph.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class RealplayStreamGrabBGR24Graph : public AVGraph
			{
			public:
				RealplayStreamGrabBGR24Graph(void);
				virtual ~RealplayStreamGrabBGR24Graph(void);

			public:
				//创建
				//@conf [in] : 配置参数
				//@Return : 错误码
				//@Comment : 固定流程
				//			1. Buffer Parser Filter
				//			2. PS Parser Filter
				//			3. Frame Decoder Filter
				//			4. Frame Converter Filter
				int createNew(const AVModeConf& conf) override;

			protected:
				int connectPin(void) override;
			};//class RealplayStreamGrabBGR24Graph
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_GRAB_BGR24_GRAPH_H
