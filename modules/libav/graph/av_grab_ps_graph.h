//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-11
//		Description : 实时流PS数据抓取流程图
//
//		History:
//					1. 2022-03-11 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_GRAB_PS_GRAPH_H
#define MODULE_AV_STREAM_AV_GRAB_PS_GRAPH_H

#include "av_graph.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVGrabPSGraph : public AVGraph
			{
			public:
				AVGrabPSGraph(void);
				virtual ~AVGrabPSGraph(void);

			public:
				//创建
				//@conf [in] : 配置参数
				//@Return : 错误码
				//@Comment : 固定流程
				//			1. Buffer Parser Filter
				int createNew(const AVModeConf& conf) override;

			protected:
				int connectPin(void) override;
			};//class AVGrabPSGraph
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_GRAB_PS_GRAPH_H
