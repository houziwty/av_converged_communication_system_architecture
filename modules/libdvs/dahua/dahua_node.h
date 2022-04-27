//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-03-17
//		Description : 大华设备节点
//
//		History:
//					1. 2022-03-17 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_DAHUA_NODE_H
#define MODULE_DEVICE_DVS_DAHUA_NODE_H

#include "dhnetsdk.h"
#include "dvs_node.h"

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class DahuaNode : public DVSNode
			{
			public:
				DahuaNode(
					PolledDataCallback data, 
					PolledExceptionCallback exception);
				virtual ~DahuaNode(void);

			public:
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
					LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, LDWORD dwUser);
				static void CALLBACK disconnectCallback(
					LLONG lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser);
				static void CALLBACK reconnectedCallback(
					LLONG lLoginID, char *pchDVRIP, LONG nDVRPort, LDWORD dwUser);

			private:
				static uint32_t counter;
				bool enableException;
			};//class DahuaNode
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_DAHUA_NODE_H
