//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-26
//		Description : ES over RTP解析器
//
//		History:
//					1. 2022-05-26 由王科威创建
//

#ifndef MODULE_AV_STREAM_RTP_ES_PARSER_H
#define MODULE_AV_STREAM_RTP_ES_PARSER_H

#include "av_parser_node.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
            class RTPESParser : public AVParserNode
            {
            public:
                RTPESParser(
                    ParsedDataCallback callback, 
					const uint32_t id = 0);
                virtual ~RTPESParser(void);

            public:
                int input(const void* avpkt = nullptr) override;

            private:
                void* rtp;
            };//class RTPESParser
        }//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_RTP_ES_PARSER_H
