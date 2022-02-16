//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : PS解析器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef MODULE_AV_STREAM_PS_PARSER_H
#define MODULE_AV_STREAM_PS_PARSER_H

#include "av_parser.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
            class PSParser : public AVParser
            {
            public:
                PSParser(
                    ParsedDataCallback callback, 
					const uint32_t id = 0);
                virtual ~PSParser(void);

            public:
                int input(
                    const void* data = nullptr, 
					const uint64_t bytes = 0) override;
            };//class PSParser
        }//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_PS_PARSER_H
