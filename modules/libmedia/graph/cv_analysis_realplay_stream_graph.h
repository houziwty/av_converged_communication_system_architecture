//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 算法分析实时视频流程图类
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_MEDIA_AV_CV_ANALYSIS_REALPLAY_STREAM_GRAPH_H
#define MODULE_MEDIA_AV_CV_ANALYSIS_REALPLAY_STREAM_GRAPH_H

#include "libmedia/graph.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class CVAnalysisRealplayStreamGraph : public Graph
			{
			public:
				CVAnalysisRealplayStreamGraph(void);
				virtual ~CVAnalysisRealplayStreamGraph(void);

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
			};//class CVAnalysisRealplayStreamGraph
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_AV_CV_ANALYSIS_REALPLAY_STREAM_GRAPH_H
