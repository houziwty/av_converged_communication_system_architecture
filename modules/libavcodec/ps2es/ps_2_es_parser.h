//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : PS流解析
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef MODULE_MEDIA_AV_PS_2_ES_PARSER_H
#define MODULE_MEDIA_AV_PS_2_ES_PARSER_H

#include "libavp/av_process.h"

namespace module
{
	namespace media
	{
		namespace av
		{
            class PS2ESParser final : public AVProcessor
            {
            public:
                PS2ESParser(void);
                ~PS2ESParser(void);

            public:
				int createNew(void* param = nullptr) override;
                int destroy(void) override;
                int input(void* data = nullptr) override;
            };//class PS2ESParser
        }//namespace av
	}//namespace media
}//namespace module

#endif//MODULE_MEDIA_AV_PS_2_ES_PARSER_H
