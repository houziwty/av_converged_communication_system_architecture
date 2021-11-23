//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-18
//		Description : 过滤器类
//
//		History:
//					1. 2021-11-18 由王科威创建
//

#ifndef MODULE_MEDIA_AV_FILTER_H
#define MODULE_MEDIA_AV_FILTER_H

#include "libavp/avp.h"
#include "defs.h"

namespace module
{
	namespace media
	{
		namespace av
		{
			using AVProcessorPtr = boost::shared_ptr<AVProcessor>;

			class Filter
			{
			public:
				Filter(const WorkMode wm = WorkMode::WORK_MODE_SOURCE);
				virtual ~Filter(void);

			public:
				//创建过滤器
				//@Return : 错误码
				virtual int createNew(void);

				//输入数据
				//@frame : 帧
				//@Return : 错误码
				virtual int input(FramePtr frame);

				//查询针脚实例
				//@name : 名称
				//@Return : 针脚实例
				PinPtr query(const std::string name);

				//获取工作模式
				//@Return : 工作模式
				inline const WorkMode getWorkMode(void) const
				{
					return mode;
				}

			protected:
				//创建处理器
				//@Return : 错误码
				virtual AVProcessorPtr createNewProcessor(void) = 0;

			protected:
				const WorkMode mode;
				PinPtrs pins;
				AVProcessorPtr avprocessor;
			};//class Filter
		}//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_AV_FILTER_H
