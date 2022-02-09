//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-27
//		Description : DVS节点
//					  多物理设备管理
//
//		History:
//					1. 2022-01-27 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_DVS_NODE_H
#define MODULE_DEVICE_DVS_DVS_NODE_H

#include <vector>
#include "defs.h"

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class DEVICE_DVS_EXPORT DVSNode
			{
			public:
				DVSNode(void);
				virtual ~DVSNode(void);

			public:
				//添加设备
				//@conf [in] : 设备配置参数
				//@Return : 错误码
				int addConf(const DVSModeConf& conf);

				//删除设备
				//@id [in] : 设备ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//查询设备配置集
				//@confs [in] : 设备配置集
				//@Return : 错误码
				int queryConf(std::vector<DVSModeConf>& confs);

				//查询设备配置
				//@id [in] : 设备ID
				//@Return : 设备配置
				const DVSModeConf& queryConf(const uint32_t id = 0);

				//运行设备
				//@id [in] : 设备ID
				//@Return : 错误码
				virtual int run(const uint32_t id = 0);

				//停止设备
				//@id [in] : 设备ID
				//@Return : 错误码
				virtual int stop(const uint32_t id = 0);

			protected:
				//实时流数据通知
				//@id [out] : 设备ID
				//@channel [out] : 通道号，从1开始计数
				//@type [out] : 数据类型
				//@data [out] : 数据 
				//@bytes [out] : 大小
				virtual void afterPolledRealplayDataNotification(
					const uint32_t id = 0, 
					const int32_t channel = 0, 
					const uint32_t type = 0, 
					const uint8_t* data = nullptr, 
					const uint32_t bytes = 0) = 0;
			};//class DVSNode
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_DVS_NODE_H
