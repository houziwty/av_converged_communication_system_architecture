//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : AV数据回调过滤器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_DATA_CALLBACK_FILTER_H
#define MODULE_AV_STREAM_AV_DATA_CALLBACK_FILTER_H

#include "filter/av_filter.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AVDataCallbackFilter : public AVFilter
			{
			public:
				AVDataCallbackFilter(void);
				virtual ~AVDataCallbackFilter(void);

			public:
				int input(const AVPkt* avpkt = nullptr) override;

				//设置回调
				//@callback : 回调函数
//				void setAVStreamCallback(AVStreamResultCallback callback);

			private:
//				AVStreamResultCallback avstreamResultCallback;
			};//class AVStreamCallbackFilter
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_DATA_CALLBACK_FILTER_H