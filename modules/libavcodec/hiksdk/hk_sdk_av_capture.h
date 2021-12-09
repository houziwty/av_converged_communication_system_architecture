//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 海康设备SDK AV捕获器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef MODULE_MEDIA_HK_SDK_AV_CAPTURE_H
#define MODULE_MEDIA_HK_SDK_AV_CAPTURE_H

#include "libavp/av_process.h"

namespace module
{
	namespace media
	{
		namespace av
		{
            typedef struct tagLoginInfo_t
            {
                unsigned short port;
                std::string ip;
                std::string username;
                std::string passwd;
            }LoginInfo;

            class HKSdkAVCapture final : public AVProcessor
            {
            public:
                HKSdkAVCapture(void);
                ~HKSdkAVCapture(void);

            public:
				int createNew(void* param = nullptr) override;
                int destroy(void) override;
                int input(void* data = nullptr) override;

            private:
                static void __stdcall captureRealplayDataCallback(
                    int playid, unsigned long long type, char* data, unsigned long long bytes, void* ctx);

            private:
                long user;
                int playId;
                static int counter;
            };//class HKSdkAVCapture
        }//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_HK_SDK_AV_CAPTURE_H
