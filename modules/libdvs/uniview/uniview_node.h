//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-23
//		Description : 宇视设备节点
//
//		History:
//					1. 2022-05-23 由王科威创建
//

#ifndef MODULE_DEVICE_DVS_UNIVIEW_NODE_H
#define MODULE_DEVICE_DVS_UNIVIEW_NODE_H

#include "dvs_node.h"

namespace module
{
	namespace device
	{
		namespace dvs
		{
			class UniviewNode : public DVSNode
			{
			public:
				UniviewNode(
					PolledDataCallback data, 
					PolledExceptionCallback exception);
				virtual ~UniviewNode(void);

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
					void* lRealHandle, const unsigned char *pBuffer, int dwBufSize, int dwMediaDataType, void* pUser);
				static void CALLBACK exceptionCallBack(
					void* lpUserID, int dwType, void* lpExpHandle, void* lpUserData, void* pstExceptionOutputInfo);

			private:
				static uint32_t counter;
			};//class UniviewNode
		}//namespace dvs
	}//namespace device
}//namespace module

#endif//MODULE_DEVICE_DVS_UNIVIEW_NODE_H
