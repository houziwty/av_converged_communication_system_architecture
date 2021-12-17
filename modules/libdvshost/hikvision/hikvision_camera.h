//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-13
//		Description : 海康DVS摄像机
//
//		History:
//					1. 2021-12-13 由王科威创建
//

#ifndef MODULE_DVS_HOST_HIKVISION_CAMERA_H
#define MODULE_DVS_HOST_HIKVISION_CAMERA_H

#include "libdvshost/camera.h"

namespace module
{
	namespace dvs
	{
		class HikvisionCamera 
			: public Camera
		{
		public:
			HikvisionCamera(
				const int idx = -1,
				const CameraType ct = CameraType::CAMERA_TYPE_ANALOGY);
			virtual ~HikvisionCamera(void);

		public:
			inline const int index(void) const override
			{
				return cameraIndex;
			}
			
			inline const CameraType type(void) const override
			{
				return cameraType;
			}

			inline void ipaddr(const std::string addr) override
			{
				digitalChannelIP = addr;
			}

			inline const std::string& ipaddr(void) const override
			{
				return digitalChannelIP;
			}

		protected:
			const int cameraIndex;
			const CameraType cameraType;
			std::string digitalChannelIP;
		};//class HikvisionCamera
	}//namespace dvs
}//namespace module

#endif//MODULE_DVS_HOST_HIKVISION_CAMERA_H
