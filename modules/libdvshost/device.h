//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-10
//		Description : DVS设备
//
//		History:
//					1. 2021-12-10 由王科威创建
//

#ifndef MODULE_DVS_HOST_DEVICE_H
#define MODULE_DVS_HOST_DEVICE_H

#include "defs.h"

namespace module
{
	namespace dvs
	{
		class DVS_HOST_EXPORT Device
		{
		public:
			Device(void);
			virtual ~Device(void);

		public:
			//登录
			//@username [in] : 用户名
			//@userpwd [in] : 密码
			//@ip [in] : IP地址
			//@port [in] : 端口号
			//@Return : 登录ID
			virtual int login(
				const std::string username, 
				const std::string userpwd, 
				const std::string ip, 
				const unsigned short port = 0) = 0;

			//注销
			//@userID [in] : 登录ID
			//@Return : 错误码
			virtual int logout(const int userID = -1) = 0;

			//获取摄像机
			//@userID [in] : 登录ID
			//@Return : 摄像机
			virtual const CameraPtrs cameras(const int userID = -1) = 0;

			//获取类型
			//@Return : 设备类型
			virtual const FactoryType type(void) = 0;
		};//class Device
	}//namespace dvs
}//namespace module

#endif//MODULE_DVS_HOST_DEVICE_H
