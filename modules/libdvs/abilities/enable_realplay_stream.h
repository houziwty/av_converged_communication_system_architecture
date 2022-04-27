//
//		Copyright : @2022, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-13
//		Description : 实时音视频流使能接口
//
//		History:
//					1. 2022-04-13 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_ENABLE_REALPLAY_STREAM_H
#define MODULE_DEVICE_DVS_ENABLE_REALPLAY_STREAM_H

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class EnableRealplayStream
			{
			public:
				EnableRealplayStream(void){}
				virtual ~EnableRealplayStream(void){}

			public:
				//打开
				//@user [in] : 用户ID
    			//@bytes [in] : 通道
				//@Return : 流ID
				virtual int64_t openRealplay(
					const int64_t uid = 0,
					const int32_t channel = -1) = 0;

				//关闭
				//@sid [in] : 流ID
				//@Return : 错误码
				virtual int closeRealplay(const int64_t sid = 0) = 0;
			};//class EnableRealplayStream
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_ENABLE_REALPLAY_STREAM_H
