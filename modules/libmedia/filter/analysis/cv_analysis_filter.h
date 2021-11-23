//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-22
//		Description : CV算法过滤器类
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_MEDIA_CV_ANALYSIS_FILTER_H
#define MODULE_MEDIA_CV_ANALYSIS_FILTER_H

#include "libmedia/filter.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			class CvAnalysisFilter : public Filter
			{
			public:
				CvAnalysisFilter(void);
				virtual ~CvAnalysisFilter(void);

			public:
				int createNew(void) override;
				int input(FramePtr frame) override;

			protected:
				AVProcessorPtr createNewProcessor(void) override;
			};//class CvAnalysisFilter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_CV_ANALYSIS_FILTER_H