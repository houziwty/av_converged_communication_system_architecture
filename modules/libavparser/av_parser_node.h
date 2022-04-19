//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-15
//		Description : AV解析器
//
//		History:
//					1. 2022-02-15 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PARSER_NODE_H
#define MODULE_AV_STREAM_AV_PARSER_NODE_H

#include "boost/function.hpp"
#include "libavparser_defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			//解析数据回调
			//@_1 [out] : 解析ID
			//@_7 [out] : 数据
			typedef boost::function<void(const uint32_t, const void*)> ParsedDataCallback;

			class AVParserNode
			{
			public:
				AVParserNode(
					ParsedDataCallback callback, 
					const uint32_t id = 0);
				virtual ~AVParserNode(void);

			public:
				//输入数据
				//@avpkt [in] : 数据包
				//@Return : 错误码
				virtual int input(const void* avpkt = nullptr) = 0;

			protected:
				const uint32_t pid;
				ParsedDataCallback parsedDataCallback;
			};//class AVParserNode
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PARSER_NODE_H
