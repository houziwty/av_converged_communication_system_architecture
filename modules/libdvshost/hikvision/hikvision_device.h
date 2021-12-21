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

#ifndef MODULE_DVS_HOST_HIKVISION_DEVICE_H
#define MODULE_DVS_HOST_HIKVISION_DEVICE_H

extern "C"
{
#include "HCNetSDK.h"
}
#include "device.h"

namespace module
{
	namespace dvs
	{
		class HikvisionDevice 
			: public Device
		{
		public:
			HikvisionDevice(void);
			virtual ~HikvisionDevice(void);

		public:
			int login(
				const std::string username, 
				const std::string userpwd, 
				const std::string ip, 
				const unsigned short port = 0) override;
			int logout(const int userID = -1) override;
			const CameraPtrs cameras(const int userID = -1) override;
			inline const FactoryType type(void) override
			{
				return FactoryType::FACTORY_TYPE_HK;
			}

		private:
			//海康SDK初始化
			//@Return : 错误码
			int initialize(void);

			//海康SDK释放
			//@Return : 错误码
			int uninitialize(void);

		private:
			static unsigned int deviceCounter;
			NET_DVR_DEVICEINFO_V40 deviceInfo;
		};//class HikvisionDevice
	}//namespace dvs
}//namespace module

#endif//MODULE_DVS_HOST_HIKVISION_DEVICE_H
