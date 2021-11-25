//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 海康设备捕获器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef FRAMEWORK_MEDIA_AV_HK_DVS_CAPTURE_H
#define FRAMEWORK_MEDIA_AV_HK_DVS_CAPTURE_H

#include "avcap/av_capture.h"

namespace framework
{
	namespace media
	{
		namespace av
		{
            class HKSdkCapture final : public AVCapture
            {
            public:
                HKSdkCapture(void);
                ~HKSdkCapture(void);

            public:
				int createNew(void) override;
                int destroy(void) override;

            private:
                long user;
            };//class HKSdkCapture
        }//namespace av
	}//namespace media
}//namespace framework

#endif//FRAMEWORK_MEDIA_AV_HK_DVS_CAPTURE_H
