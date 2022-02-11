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
// 							Data type			uint32_t			4Bytes	
// 							Stream type			uint32_t			4Bytes	
// 							Frame type			uint32_t			4Bytes	
// 							Frame size			uint32_t			4Bytes	
// 							Frame sequence		uint64_t			8Bytes	
// 							Frame timestamp		uint64_t			8Bytes	
// 							Frame data			uint8_t*			nBytes
//
//		History:
//					1. 2022-02-11 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_BUFFER_BUFFER_PARSER_H
#define FRAMEWORK_NETWORK_BUFFER_BUFFER_PARSER_H

#include <stdint.h>
#include "boost/function.hpp"

namespace framework
{
	namespace network
	{
		namespace buffer
		{
			//解析单帧数据回调
			//@_1 [out] : 数据类型
			//@_2 [out] : 流类型
			//@_3 [out] : 帧类型
			//@_4 [out] : 帧大小
			//@_5 [out] : 帧序号
			//@_6 [out] : 帧时间戳
			//@_7 [out] : 帧数据
			typedef boost::function<void(const uint32_t, const uint32_t, const uint32_t, const uint32_t, const uint64_t, const uint64_t, const uint8_t*)> ParsedOneFrameDataCallback;
			const static uint32_t frame_header_flag = 0xff050301;
			const static uint32_t frame_header_size = 36;

			class BufferParser
			{
			public:
				//bytes [in] : 缓存大小
				BufferParser(
					ParsedOneFrameDataCallback callback, 
					const uint64_t bytes = 3 * 1024 * 1024);
				virtual ~BufferParser(void);

			public:
				//添加数据
				//@data [in] : 数据
				//@bytes [in] : 数据大小
				//@Return : 错误码
				int append(
					const uint8_t* data = nullptr, 
					const uint64_t bytes = 0);

			private:
				//解析单帧数据
				int parse(void);

			private:
				ParsedOneFrameDataCallback parsedOneFrameDataCallback;
				uint8_t* buffer;
				const uint64_t bufSize;
				uint64_t pos;
			};//class BufferParser
		}//namespace buffer
	}//namespace network
}//namespace framework

#endif//FRAMEWORK_NETWORK_BUFFER_BUFFER_PARSER_H
