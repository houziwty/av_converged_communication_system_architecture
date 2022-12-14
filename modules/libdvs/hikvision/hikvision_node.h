//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-10
//		Description : 海康设备节点
//
//		History:
//					1. 2021-12-10 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_HIKVISION_NODE_H
#define MODULE_DEVICE_DVS_HIKVISION_NODE_H

#include "dvs_node.h"

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class HikvisionNode : public DVSNode
			{
			public:
				HikvisionNode(
					PolledDataCallback data, 
					PolledExceptionCallback exception);
				virtual ~HikvisionNode(void);

			public:
				void afterFetchExceptionNotification(const uint32_t type = 0);

			protected:
				int64_t login(
					const char* ip = nullptr, 
					const uint16_t port = 0, 
					const char* user = nullptr, 
					const char* passwd = nullptr) override;
				int logout(const int64_t uid = -1) override;
				int catchException(void) override;
				int64_t openRealplay(
					const int64_t uid = 0, 
					const int32_t channel = -1) override;
				int closeRealplay(const int64_t sid = 0) override;
				int getChanNum(
					const int64_t uid, 
					std::vector<int64_t>& chanNums) override;

			private:
				static void CALLBACK livestreamDataCallback(
					int lRealHandle, unsigned int dwDataType, unsigned char *pBuffer, unsigned int dwBufSize, void* pUser);

			private:
				static uint32_t counter;
			};//class HikvisionNode
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_HIKVISION_NODE_H
