//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : AV流回调过滤器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef MODULE_AV_STREAM_CALLBACK_FILTER_H
#define MODULE_AV_STREAM_CAPTURE_FILTER_H

#include "libavstream/av_stream_filter.h"

namespace module
{
	namespace avstream
	{
		class AV_STREAM_EXPORT AVStreamCallbackFilter 
			: public AVFilter
		{
		public:
			AVStreamCallbackFilter(void);
			virtual ~AVStreamCallbackFilter(void);

		public:
			int input(void* data = nullptr) override;

			//设置回调
			//@callback : 回调函数
			void setAVStreamCallback(AVStreamResultCallback callback);

		private:
			AVStreamResultCallback avstreamResultCallback;
		};//class AVStreamCallbackFilter
	}//namespace avstream
}//namespace module

#endif//MODULE_AV_STREAM_CAPTURE_FILTER_H