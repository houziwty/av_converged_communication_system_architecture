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

#include <vector>
#include "boost/function.hpp"
#include "defs.h"

namespace module
{
	namespace device
	{
		namespace dvs
		{
			typedef boost::function<void(const uint32_t, const int32_t, const uint32_t, const uint8_t*, const uint32_t)> PolledDataCallback;

			class Device
			{
			public:
				Device(
					const DVSModeConf& conf, 
					PolledDataCallback callback);
				virtual ~Device(void);

			public:
				//运行
				//@Return : 错误码
				virtual int run(void) = 0;

				//停止
				//@Return : 错误码
				virtual int stop(void) = 0;

				//获取摄像机索引号
				//@Return : 摄像机索引号
				inline const std::vector<int32_t> cameras(void) const
				{
					return indexes;
				}

			protected:
				const DVSModeConf modeconf;
				std::vector<int32_t> indexes;
				std::vector<int32_t> streams;
				PolledDataCallback polledDataCallback;
			};//class Device
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_DEVICE_H
