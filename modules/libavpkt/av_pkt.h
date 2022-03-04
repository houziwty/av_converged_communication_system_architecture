//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-14
//		Description : AV数据包
//
//		History:
//					1. 2021-12-14 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PKT_H
#define MODULE_AV_STREAM_AV_PKT_H

#include "defs.h"

namespace module
{
    namespace av
    {
        namespace stream
        {
            class AV_PKT_EXPORT AVPkt
            {
            public:
                //@mainType [in] : 主类型
				//@subType [in] : 子类型
				//@sequence [in] : 序列号
                //@timestamp [in] : 时间戳
                AVPkt(
                    const AVMainType mainType = AVMainType::AV_MAIN_TYPE_NONE, 
                    const AVSubType subType = AVSubType::AV_SUB_TYPE_NONE, 
                    const uint64_t sequence = 0, 
                    const uint64_t timestamp = 0, 
                    const uint32_t w = 0, 
                    const uint32_t h = 0);
                virtual ~AVPkt(void);

            public:
                //输入数据
				//@data [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
                //@Common : 重复数据会覆盖
                int input(
					const void* data = nullptr, 
					const uint64_t bytes = 0);

                //获取数据
                //@Return : 数据
                inline const void* data(void) const
                {
                    return avpktData;
                }

                //获取数据大小
                //@Return : 数据大小
                inline const uint64_t bytes(void) const
                {
                    return avpktDataBytes;
                }

                //获取主类型
                //@Return : 主类型
                inline const AVMainType maintype(void) const
                {
                    return avpktMainType;
                }

                //获取子类型
                //@Return : 子类型
                inline const AVSubType subtype(void) const
                {
                    return avpktSubType;
                }

                //获取序列号
                //@Return : 序列号
                inline const uint64_t sequence(void) const
                {
                    return avpktSequence;
                }

                //获取时间戳
                //@Return : 时间戳
                inline const uint64_t timestamp(void) const
                {
                    return avpktTimestamp;
                }

				//获取宽
				//@Return : 宽
				inline const uint32_t width(void) const
				{
					return pixelWidth;
				}

				//获取高
				//@Return : 高
				inline const uint32_t height(void) const
				{
					return pixelHeight;
				}

            private:
                const AVMainType avpktMainType;
                const AVSubType avpktSubType;
                const uint64_t avpktSequence;
                const uint64_t avpktTimestamp;
                void* avpktData;
                uint64_t avpktDataBytes;
                const uint32_t pixelWidth;
                const uint32_t pixelHeight;
            };//class AVPkt
        }//namespace stream
    }//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PKT_H