//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-11
//		Description : 会话
//
//		History:
//					1. 2022-05-11 由王科威创建
//

#ifndef FRAMEWORK_NETWORK_SESSION_SESSION_H
#define FRAMEWORK_NETWORK_SESSION_SESSION_H

#include <stdint>

namespace framework
{
    namespace network
    {
        namespace session
        {
            class Session
            {
            public:
                Session(const uint32_t id = 0);
                virtual ~Session(void);

            public:
                //获取会话ID
                //@Return : 会话ID
                inline const uint32_t id(void) const
                {
                    return sessionid;
                }

            private:
                const uint32_t sessionid;
            };//class Session
        }//namespace session
    }//namespace network
}//namespace framework

#endif//SERVICE_DVS_STREAM_SESSION_H