//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-10
//		Description : 海康DVS设备
//
//		History:
//					1. 2021-12-10 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_HIKVISION_DEVICE_H
#define MODULE_DEVICE_DVS_HIKVISION_DEVICE_H

#include "device.h"

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class HikvisionDevice : public Device
			{
			public:
				HikvisionDevice(
					const DVSModeConf& conf, 
					PolledRealplayDataCallback callback);
				virtual ~HikvisionDevice(void);

			public:
				int run(void) override;
				int stop(void) override;

			private:
				//打开所有摄像机实时主码流
				//@param [in] : 参数
				//@Return : 错误码
				int openRealplayStream(void* param = nullptr);

				//关闭所有摄像机实时主码流
				//@Return : 错误码
				int closeRealplayStream(void);

				//设备数据流获取回调函数
				//@sid [out] : 流ID
				//@type [out] : 数据类型
				//@data [out] : 数据 
				//@bytes [out] : 大小
				//@user [out] : 用户参数
				static void CALLBACK livestreamDataCallback(
					int32_t sid, uint32_t type, void* data, uint32_t bytes, void* user);

			private:
				static uint32_t counter;
				int32_t user;
			};//class HikvisionDevice
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_HIKVISION_DEVICE_H
