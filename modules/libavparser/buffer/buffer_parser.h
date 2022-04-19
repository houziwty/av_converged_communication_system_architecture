//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-11
//		Description : 缓存数据解析
//						单帧数据格式定义如下：
//							字段名				 类型				  大小			  默认值
//							Flag				uint32_t  			4Bytes	  		0xFF050301
// 							Main type			uint32_t			4Bytes	
// 							Sub type			uint32_t			4Bytes	
// 							Frame size			uint32_t			4Bytes	
// 							Frame sequence		uint64_t			8Bytes	
// 							Frame timestamp		uint64_t			8Bytes	
// 							Frame data			void*			nBytes
//
//		History:
//					1. 2022-02-11 由王科威创建
//

#ifndef MODULE_AV_STREAM_BUFFER_PARSER_H
#define MODULE_AV_STREAM_BUFFER_PARSER_H

#include "av_parser_node.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			const static uint32_t frame_header_flag = 0xff050301;
			const static uint32_t frame_header_size = 32;

			class BufferParser : public AVParserNode
			{
			public:
				//callback [in] : 数据解析回调
				//id [in] : 解析ID
				//bytes [in] : 缓存大小
				BufferParser(
					ParsedDataCallback callback, 
					const uint32_t id = 0, 
					const uint64_t bytes = 3 * 1024 * 1024);
				virtual ~BufferParser(void);

			public:
				//输入数据
				//@avpkt [in] : 数据包
				//@Return : 错误码
				int input(const void* avpkt = nullptr) override;

			private:
				//解析单帧数据
				int parse(void);

			private:
				void* buffer;
				const uint64_t bufSize;
				uint64_t pos;
			};//class BufferParser
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_BUFFER_PARSER_H
