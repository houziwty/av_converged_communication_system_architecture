//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-17
//		Description : 大华DVS设备
//
//		History:
//					1. 2022-03-17 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_DAHUA_DEVICE_H
#define MODULE_DEVICE_DVS_DAHUA_DEVICE_H

extern "C"
{
#include "dhnetsdk.h"
}
#include "device.h"

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class DahuaDevice : public Device
			{
			public:
				DahuaDevice(
					const DVSModeConf& conf, 
					PolledRealplayDataCallback callback);
				virtual ~DahuaDevice(void);

			public:
				int run(void) override;
				int stop(void) override;

			private:
				//设备数据流获取回调函数
				//@lRealHandle [out] : 流ID
				//@dwDataType [out] : 数据类型
				//@pBuffer [out] : 数据 
				//@dwBufSize [out] : 大小
				//@param [out] : 用户参数
				//@dwUser [out] : 用户参数
				static void CALLBACK livestreamDataCallback(
					LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, LDWORD dwUser);

			private:
				static uint32_t counter;
				LLONG user;
			};//class DahuaDevice
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_DAHUA_DEVICE_H
