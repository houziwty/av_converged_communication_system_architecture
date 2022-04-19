//
//		Copyright : @2022, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-13
//		Description : 摄像机配置使能接口
//
//		History:
//					1. 2022-04-13 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_ENABLE_CONFIG_CAMERA_H
#define MODULE_DEVICE_DVS_ENABLE_CONFIG_CAMERA_H

#include <vector>

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class EnableConfigCamera
			{
			public:
				EnableConfigCamera(void){}
				virtual ~EnableConfigCamera(void){}

			public:
				//获取通道号
				//@user [in] : 用户ID
    			//@chanNums [out] : 通道号
				//@Return : 错误码
				virtual int getChanNum(
					const int64_t uid, 
					std::vector<int64_t>& chanNums) = 0;
			};//class EnableConfigCamera
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_ENABLE_CONFIG_CAMERA_H
