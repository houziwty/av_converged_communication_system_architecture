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

#include <string>
#include "boost/shared_ptr.hpp"
#include "boost/unordered_map.hpp"

namespace module
{
	namespace media
	{
		namespace av
		{
			//过滤器工作模式
			//必选其一
			typedef enum class tagWorkMode_t : int
			{
				WORK_MODE_SOURCE = 0, 
				WORK_MODE_MEDIUM,
				WORK_MODE_TARGET
			}WorkMode;

			static const std::string innerVideoDecoderFilterName = "InnerVideoDecoderFilter";
			static const std::string innerVideoRenderFilterName = "InnerVideoRenderFilter";
			static const std::string innerAudioDecoderFilterName = "InnerAudioDecoderFilter";
			static const std::string innerAudioPlayerFilterName = "InnerAudioPlayerFilter";
			static const std::string innerImageFormatterFilterName = " innerImageFormatterFilter";
			static const std::string innerPictureEncoderFilterName = " innerPictureEncoderFilter";
			static const std::string innerCVAnalysisFilterName = " innerCVAnalysisFilter";

			class Pin;
            using PinPtr = boost::shared_ptr<Pin>;
			using PinPtrs = boost::unordered_map<const std::string, PinPtr>;
			class AVProcessor;
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
