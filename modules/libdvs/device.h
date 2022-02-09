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

#ifndef MODULE_DEVICE_DVS_DEVICE_H
#define MODULE_DEVICE_DVS_DEVICE_H

#include "boost/function.hpp"
#include "defs.h"
#include "utils/map/unordered_map.h"

namespace module
{
	namespace device
	{
		namespace dvs
		{
			//实时流数据回调
			//@_1 : 设备ID
			//@_2 : 通道号
			//@_3 : 数据类型
			//@_4 : 数据
			//@_5 : 数据大小
			typedef boost::function<void(const uint32_t, const int32_t, const uint32_t, const uint8_t*, const uint32_t)> PolledRealplayDataCallback;

			class Device
			{
			public:
				Device(
					const DVSModeConf& conf, 
					PolledRealplayDataCallback callback);
				virtual ~Device(void);

			public:
				//获取设备配置
				//@Return : 设备配置
				inline const DVSModeConf& getConf(void) const
				{
					return modeconf;
				}

				//运行
				//@Return : 错误码
				virtual int run(void) = 0;

				//停止
				//@Return : 错误码
				virtual int stop(void) = 0;

			protected:
				DVSModeConf modeconf;
				UnorderedMap<int32_t, int32_t> livestreamIds;
				PolledRealplayDataCallback polledRealplayDataCallback;
			};//class Device
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_DEVICE_H
