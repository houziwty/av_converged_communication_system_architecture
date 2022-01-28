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

				//运行
				//@Return : 错误码
				virtual int run(void);

				//停止
				//@Return : 错误码
				virtual int stop(void);

			protected:
				//接收流数据通知
				//@dvs [out] : 设备ID
				//@strea [out] : 流ID
				//@type [out] : 数据类型
				//@data [out] : 数据 
				//@bytes [out] : 大小
				virtual void afterPolledRealDataNotification(
					const uint32_t dvs = 0, 
					const int32_t stream = -1, 
					const uint32_t type = 0, 
					const uint8_t* data = nullptr, 
					const uint32_t bytes = 0) = 0;
			};//class DVSNode
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_DVS_NODE_H
