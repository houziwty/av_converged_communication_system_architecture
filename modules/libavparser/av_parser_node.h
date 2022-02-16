//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-15
//		Description : AV数据解析节点
//						1.PS转PES
//						2.缓存解析
//
//		History:
//					1. 2021-11-22 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PARSER_NODE_H
#define MODULE_AV_STREAM_AV_PARSER_NODE_H

#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			class AV_PARSER_EXPORT AVParserNode
			{
			public:
				AVParserNode(void);
				virtual ~AVParserNode(void);

			public:
				//添加角色
				//@conf [in] : 角色配置参数
				//@Return : 错误码
				int addConf(const AVParserModeConf& conf);

				//删除角色
				//@id [in] : 角色ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//输入数据
				//@id [in] : 角色ID
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				virtual int input(
					const uint32_t id = 0, 
					const void* data = nullptr, 
					const uint64_t bytes = 0) = 0;

			protected:
				//解析数据通知
				//@id [out] : 角色ID
				//@data [out] : 主类型
				//@data [out] : 子类型
				//@bytes [out] : 大小
				//@seqence [out] : 序号
				//@timestamp [out] : 时间戳
				//@data [out] : 数据
				virtual void afterParsedDataNotification(
					const uint32_t id = 0, 
					const MainType maintype = MainType::MAIN_TYPE_NONE, 
					const SubType subtype = SubType::SUB_TYPE_NONE, 
					const uint32_t bytes = 0, 
					const uint64_t seqence = 0, 
					const uint64_t timestamp = 0, 
					const void* data = nullptr) = 0;
			};//class AVParserNode
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PARSER_NODE_H
