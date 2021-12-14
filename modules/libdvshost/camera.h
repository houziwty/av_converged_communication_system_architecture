//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-10
//		Description : DVS摄像机
//
//		History:
//					1. 2021-12-10 由王科威创建
//

#ifndef MODULE_DVS_HOST_CAMERA_H
#define MODULE_DVS_HOST_CAMERA_H

#include "defs.h"

namespace module
{
	namespace dvs
	{
		class DVS_HOST_EXPORT Camera
		{
		public:
			Camera(void);
			virtual ~Camera(void);

		public:
			//获取索引号
			//@Return : 索引号
			virtual const int index(void) const = 0;

			//获取类型
			//@Return : 索引号
			virtual const CameraType type(void) const = 0;

			//设置数字通道IP地址
			//@addr [in] : IP地址
			virtual void ipaddr(const std::string ip) = 0;

			//获取数字通道IP地址
			//@Return : IP地址
			virtual const std::string& ipaddr(void) const = 0;
		};//class Camera
	}//namespace dvs
}//namespace module

#endif//MODULE_DVS_HOST_CAMERA_H
