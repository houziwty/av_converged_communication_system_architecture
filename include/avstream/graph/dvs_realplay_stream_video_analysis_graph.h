//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : 基于海康DVS实时流视频分析流程图类
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef FRAMEWORK_AV_STREAM_REALPLAY_VIDEO_ANALYSIS_GRAPH_H
#define FRAMEWORK_AV_STREAM_REALPLAY_VIDEO_ANALYSIS_GRAPH_H

#include "libavstream/graph/realplay_stream_video_analysis_graph.h"
using namespace module::avstream;

namespace framework
{
	namespace avstream
	{
		namespace graph
		{
			class DvsRealplayStreamVideoAnalysisGraph 
				: public RealplayStreamVideoAnalysisGraph
			{
			public:
				DvsRealplayStreamVideoAnalysisGraph(void);
				virtual ~DvsRealplayStreamVideoAnalysisGraph(void);

			public:
				//创建
				//@param : 参数
				//@Return : 错误码
				//@Comment : 算法分析实时视频流程图的构建内容包括 ：
				//						1. AV Source Filter
				//						2. AV Splitter Filter
				//						3. AV Target Filter
				int createNew(void* param = nullptr) override;

				//设置回调
				//@callback : 回调函数
				int setAVStreamCallback(AVStreamResultCallback callback);

			protected:
				int connectPin(void) override;
			};//class DvsRealplayStreamVideoAnalysisGraph
		}//namespace graph
	}//namespace avstream
}//namespace framework

#endif//FRAMEWORK_AV_STREAM_REALPLAY_VIDEO_ANALYSIS_GRAPH_H
