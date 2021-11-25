//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : AV数据源捕获器
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef FRAMEWORK_MEDIA_AV_AV_CAPTURE_H
#define FRAMEWORK_MEDIA_AV_AV_CAPTURE_H

#include <string>

namespace framework
{
	namespace media
	{
		namespace av
		{
            class AVCapture
            {
            public:
                AVCapture(void);
                virtual ~AVCapture(void);

            public:
                //创建
				//@Return : 错误码
				virtual int createNew(void) = 0;

                //销毁
                //@Return : 错误码
                virtual int destroy(void) = 0;

                //解析
                //@url : 信令
				//@Return : 错误码
                //@Comment : URL示例
                //                     1. config://192.168.2.225:8000?username=admin&password=Vrc123456&type=0
                //                     2. capture://192.168.2.225:8000?channel=1&type=0
                //                     3. record://192.168.2.225:8000?channel=1
//				virtual int set(const std::string url) = 0;
            };//class AVCapture
        }//namespace av
	}//namespace media
}//namespace framework

#endif//FRAMEWORK_MEDIA_AV_AV_CAPTURE_H