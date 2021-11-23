//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : 基于海康SDK的算法分析实时视频流程图类
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef FRAMEWORK_MEDIA_AV_HK_CV_ANALYSIS_REALPLAY_STREAM_GRAPH_H
#define FRAMEWORK_MEDIA_AV_HK_CV_ANALYSIS_REALPLAY_STREAM_GRAPH_H

#include "libmedia/graph/cv_analysis_realplay_stream_graph.h"
using namespace module::media::av;

namespace framework
{
	namespace media
	{
		namespace av
		{
			class HKCVAnalysisRealplayStreamGraph 
				: public CVAnalysisRealplayStreamGraph
			{
			public:
				HKCVAnalysisRealplayStreamGraph(void);
				virtual ~HKCVAnalysisRealplayStreamGraph(void);

			public:
				//创建流程图
				//@Return : 错误码
				//@Comment : 算法分析实时视频流程图的构建内容包括 ：
				//						1. Video Decoder Filter
				//						2. YUV to BGR24 Filter
				//						3. CV Analysis Filter
				//						4. Picture Encoder Filter
				//						5. Target Filter
				int createNew(void) override;

			protected:
				int connectPin(void) override;
			};//class HKCVAnalysisRealplayStreamGraph
		}//namespace av
	}//namespace media
}//namespace framework

#endif//FRAMEWORK_MEDIA_AV_HK_CV_ANALYSIS_REALPLAY_STREAM_GRAPH_H
