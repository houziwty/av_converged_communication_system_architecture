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

#include "av/sps/sps_parser.h"
#include "av_parser_node.h"

namespace module
{
	namespace av
	{
		namespace stream
		{
            class PSParser : public AVParserNode
            {
            public:
                PSParser(
                    ParsedDataCallback callback, 
					const uint32_t id = 0);
                virtual ~PSParser(void);

            public:
                int input(const void* avpkt = nullptr) override;

            private:
                //PS数据解析回调
                static int parsedPSPacketCallback(
                    void* param, 
                    int stream, 
                    int codecid, 
                    int flags, 
                    int64_t pts, 
                    int64_t dts, 
                    const void* data, 
                    size_t bytes);

            private:
                void* demuxer;
                uint64_t sequence;
                SPSParser spsParser;
                uint32_t width;
                uint32_t height;
            };//class PSParser
        }//namespace stream
	}//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_PS_PARSER_H
