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

#ifndef MODULE_DEVICE_DVS_DVS_NODE_H
#define MODULE_DEVICE_DVS_DVS_NODE_H

#include "boost/function.hpp"
#include "ability/enable_login_logout.h"
#include "ability/enable_catch_exception.h"
#include "ability/enable_realplay_stream.h"

namespace module
{
	namespace device
	{
		namespace dvs
		{
			//音视频流数据回调
			//@_1 : 设备ID
			//@_2 : 通道ID
			//@_3 : 数据类型
			//@_4 : 数据
			//@_5 : 数据大小
			typedef boost::function<void(const uint32_t, const int32_t, const uint32_t, const void*, const uint32_t)> PolledDataCallback;

			//设备异常回调
			//@_1 : 设备ID
			//@_2 : 错误码
			typedef boost::function<void(const uint32_t, const int32_t)> PolledExceptionCallback;

			class DVSNode : 
				protected EnableLoginAndLogout, 
				protected EnableCatchException, 
				protected EnableRealplayStream
			{
			public:
				DVSNode(
					PolledDataCallback data, 
					PolledExceptionCallback exception);
				virtual ~DVSNode(void);

			public:
				//运行
				//@conf [in] : 设备配置
				//@Return : 错误码
				int run(const DVSModeConf& conf);

				//停止
				//@Return : 错误码
				int stop(void);

			protected:
				DVSModuleType module;
				uint32_t did;
				int64_t uid;
				//Max support channel number is 64.
				int64_t cid[64];
				PolledDataCallback polledDataCallback;
				PolledExceptionCallback polledExceptionCallback;
			};//class DVSNode
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_DVS_NODE_H
