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

#ifndef MODULE_AV_STREAM_AV_PARSER_H
#define MODULE_AV_STREAM_AV_PARSER_H

#include <cstdint>
#include "boost/function.hpp"
#include "defs.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
			//解析单帧数据回调
			//@_1 [out] : 解析ID
			//@_2 [out] : 主类型
			//@_3 [out] : 子类型
			//@_4 [out] : 大小
			//@_5 [out] : 序号
			//@_6 [out] : 时间戳
			//@_7 [out] : 数据
			typedef boost::function<void(const uint32_t, const MainType, const SubType, const uint32_t, const uint64_t, const uint64_t, const uint8_t*)> ParsedDataCallback;

			class AVParser
			{
			public:
				AVParser(
					ParsedDataCallback callback, 
					const uint32_t id = 0);
				virtual ~AVParser(void);

			public:
				//输入数据
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				virtual int input(
					const uint8_t* data = nullptr, 
					const uint64_t bytes = 0) = 0;

			protected:
				const uint32_t pid;
				ParsedDataCallback parsedDataCallback;
			};//class AVParser
		}//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PARSER_H
