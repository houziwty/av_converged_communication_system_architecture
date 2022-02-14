//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-14
//		Description : AV流图节点
//					  1.动态创建角色
//					  2.多角色并行
//
//		History:
//					1. 2022-02-14 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_STREAM_NODE_H
#define MODULE_AV_STREAM_AV_STREAM_NODE_H

#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AV_STREAM_EXPORT AVStreamNode
			{
			public:
				AVStreamNode(void);
				virtual ~AVStreamNode(void);

			public:
				//添加角色
				//@conf [in] : 角色配置参数
				//@Return : 错误码
				int addConf(const AVModeConf& conf);

				//删除角色
				//@id [in] : 会话ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//发送数据
				//@id [in] : 会话ID
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@ip [in] : 远程IP
				//@port [in] : 远程端口号
				//@Return : 错误码
				//@Comment : ip和port参数仅当UDP会话时有效
				virtual int input(
					const uint32_t id = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0);
			};//class AVStreamNode
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_STREAM_NODE_H
