//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-27
//		Description : DVS库
//
//		History:
//					1. 2022-01-27 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_LIB_DVS_H
#define MODULE_DEVICE_DVS_LIB_DVS_H

#include "libdvs_defs.h"

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class DEVICE_DVS_EXPORT Libdvs
			{
			public:
				Libdvs(void);
				virtual ~Libdvs(void);

			public:
				//添加设备
				//@conf [in] : 设备配置
				//@Return : 错误码
				int addConf(const DVSModeConf& conf);

				//删除设备
				//@id [in] : 设备ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//查询配置
				//@id [in] : 设备ID
				//@type [in] : 配置类型
				//@param [in] : 配置值
				//@Return : 错误码
				int queryConf(
					const uint32_t id = 0, 
					const DVSConfParamType type = DVSConfParamType::DVS_CONF_PARAM_TYPE_NONE, 
					void* param = nullptr);

			protected:
				//音视频流数据通知
				//@id [out] : 设备ID
				//@channel [out] : 通道ID
				//@type [out] : 数据类型
				//@data [out] : 数据 
				//@bytes [out] : 大小
				virtual void afterPolledDVSDataNotification(
					const uint32_t id = 0, 
					const int32_t channel = 0, 
					const uint32_t type = 0, 
					const void* data = nullptr, 
					const uint32_t bytes = 0) = 0;

				//设备异常状态通知
				//@id [out] : 设备ID
				//@error [out] : 错误码
				virtual void afterPolledDVSExceptionNotification(
					const uint32_t id = 0, 
					const int32_t error = 0) = 0;
			};//class Libdvs
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_LIB_DVS_H
