//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 实时流视频分析流程图类
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_AV_REALPLAY_STREAM_VIDEO_ANALYSIS_GRAPH_H
#define MODULE_AV_REALPLAY_STREAM_VIDEO_ANALYSIS_GRAPH_H

#include "av_stream_graph.h"

namespace module
{
	namespace avstream
	{
		class AV_STREAM_EXPORT RealplayStreamVideoAnalysisGraph 
			: public AVGraph
		{
		public:
			RealplayStreamVideoAnalysisGraph(void);
			virtual ~RealplayStreamVideoAnalysisGraph(void);

		public:
			//创建
			//@param : 参数
			//@Return : 错误码
			//@Comment : 算法分析实时视频流程图的构建内容包括 ：
			//						1. Video Decoder Filter
			//						2. YUV to BGR24 Filter
			//						3. CV Analysis Filter
			//						4. Picture Encoder Filter
			//						5. Target Filter
			int createNew(void* param = nullptr) override;

		protected:
			int connectPin(void) override;
		};//class RealplayStreamVideoAnalysisGraph
	}//namespace avstream
}//namespace module

#endif//MODULE_AV_REALPLAY_STREAM_VIDEO_ANALYSIS_GRAPH_H
