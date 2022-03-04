//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-02
//		Description : SPS解析器
//
//		History:
//					1. 2022-03-02 由王科威创建
//

#ifndef MODULE_AV_STREAM_SPS_PARSER_H
#define MODULE_AV_STREAM_SPS_PARSER_H

#include <cstdint>

namespace module
{
    namespace av
    {
        namespace stream
        {
            typedef struct tagSPSInfo_t
            {
                uint32_t profile_idc;
                uint32_t level_idc;
                uint32_t width;
                uint32_t height;
                uint32_t fps;
            }SPSInfo;

			typedef struct tagSPSBitStream
			{
				uint8_t* data;
                uint32_t bytes;
				uint32_t pos;//计算位标记
			}SPSBitStream;

            class SPSParser
            {
            public:
				SPSParser(void);
                ~SPSParser(void);

            public:
				//解析SPS数据信息
			    //@data [in] : 数据
			    //@bytes [in] : 大小
			    //@info [out] : SPS
			    //@Return : 错误码
				const SPSInfo h264_parse_sps(
					const uint8_t* data = nullptr,
					const uint32_t bytes = 0);

            private:
                const bool eof(SPSBitStream& bs);

				//去除NAL起始标识
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 值
                //@Comment ：支持00 00 00 01和00 00 01
				uint32_t del_nal_prefix(
					const uint8_t* data = nullptr,
					const uint32_t bytes = 0);

				//读取从偏移位的n个8位的值
                //@bs [in] : 数据
                //@number [in] : 个数
                //@Return : 值
                uint32_t u(
                    SPSBitStream& bs,
                    const uint8_t number = 0);

                //读取无符号哥伦布编码值(UE) #2^LeadingZeroBits - 1 + (xxx)
                //@bs [in] : 数据
                //@Return : 值
                uint32_t ue(SPSBitStream& bs);

				//读取有符号哥伦布编码值(UE) #(-1)^(k+1) * Ceil(k/2)
				//@bs [in] : 数据
                //@Return : 值
                int32_t se(SPSBitStream& bs);

                //视频可用性信息(Video usability information)解析
				//@bs [in] : 数据
				//@info [out] : SPS
                //@Comment : see E.1.1 VUI parameters syntax
                void vui_para_parse(SPSBitStream& bs, SPSInfo& info);
            };//class SPSParser
        }//namespace stream
    }//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_SPS_PARSER_H