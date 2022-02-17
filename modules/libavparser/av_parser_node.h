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
			class AVPkt;

			class AV_PARSER_EXPORT AVParserNode
			{
			public:
				AVParserNode(void);
				virtual ~AVParserNode(void);

			public:
				//添加解析
				//@conf [in] : 解析配置参数
				//@Return : 错误码
				int addConf(const AVParserModeConf& conf);

				//删除解析
				//@id [in] : 解析ID
				//@Return : 错误码
				int removeConf(const uint32_t id = 0);

				//输入数据
				//@id [in] : 解析ID
				//@avpkt [in] : 数据包
				//@Return : 错误码
				int input(
					const uint32_t id = 0, 
					const AVPkt* avpkt = nullptr);

			protected:
				//解析数据通知
				//@id [out] : 解析ID
				//@avpkt [in] : 数据包
				virtual void afterParsedDataNotification(
					const uint32_t id = 0, 
					const AVPkt* avpkt = nullptr) = 0;
			};//class AVParserNode
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PARSER_NODE_H
