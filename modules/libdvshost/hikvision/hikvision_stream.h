//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-13
//		Description : 海康DVS流
//
//		History:
//					1. 2021-12-13 由王科威创建
//

#ifndef MODULE_DVS_HOST_HIKVISION_STREAM_H
#define MODULE_DVS_HOST_HIKVISION_STREAM_H

#include "HCNetSDK.h"
#include "libdvshost/stream.h"

namespace module
{
	namespace dvs
	{
		class HikvisionStream 
			: public Stream
		{
		public:
			HikvisionStream(void);
			virtual ~HikvisionStream(void);

		public:
			inline const StreamType type(void) const
			{
				return streamType;
			}
			int open(
				const int userID = -1, 
				const int channel = -1, 
				const unsigned int type = 0) override;
			int close(const int streamID = -1) override;

		private:
			static void CALLBACK livestreamDataCallback(
				LONG lPlayHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser);
		
		private:
			StreamType streamType;
		};//class HikvisionStream
	}//namespace dvs
}//namespace module

#endif//MODULE_DVS_HOST_HIKVISION_STREAM_H
