//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-10
//		Description : 设备主机创建器
//
//		History:
//					1. 2021-12-10 由王科威创建
//

#ifndef MODULE_DVS_HOST_CREATOR_H
#define MODULE_DVS_HOST_CREATOR_H

#include "defs.h"

namespace module
{
	namespace dvs
	{
		class DVS_HOST_EXPORT DvsHostCreator
		{
		public:
			DvsHostCreator(void);
			~DvsHostCreator(void);

		public:
			//创建设备
			//@type [in] : 设备厂商
			//@Return : 设备实例
			DevicePtr createNewDevice(
				const FactoryType type = FactoryType::FACTORY_TYPE_NONE);

			//销毁设备
			//@dvs [in] : 设备实例
			//@Return [in] : 错误码
			int destroyDevice(DevicePtr dvs);

			//获取摄像机
			//@dvs : 设备实例
			//@Return : 摄像机实例
			const CameraPtrs cameras(const DevicePtr dvs);

			//创建流
			//@factory [in] : 设备厂商
			//@Return : 流实例
			StreamPtr createNewStream(
				const FactoryType factory = FactoryType::FACTORY_TYPE_NONE);

			//销毁流
			//@stream [in] : 流实例
			//@Return : 错误码
			int destroyStream(StreamPtr stream);
		};//class DvsHostCreator
	}//namespace dvs
}//namespace module

#endif//MODULE_DVS_HOST_CREATOR_H
