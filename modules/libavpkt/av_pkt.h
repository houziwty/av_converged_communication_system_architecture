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
                AVPkt(const AVPktType type = AVPktType::AV_PKT_TYPE_NONE);
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

                //关联数据
                //@pkt [in] : 数据包
                //@Return : 错误码
                int associate(AVPkt* pkt = nullptr);

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

                //获取数据包类型
                //@Return : 数据包类型
                inline const AVPktType type(void) const
                {
                    return avpktType;
                }

            private:
                const AVPktType avpktType;
                void* avpktData;
                uint64_t avpktDataBytes;
                AVPkt* avpkt;
            };//class AVPkt
        }//namespace stream
    }//namespace av
}//namespace module

#endif//MODULE_AV_STREAM_AV_PKT_H