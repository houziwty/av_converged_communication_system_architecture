#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "hikvision_camera.h"
#include "hikvision_device.h"
using namespace module::dvs;

unsigned int HikvisionDevice::deviceCounter = 0;

HikvisionDevice::HikvisionDevice()
	: Device(), deviceInfo{0}
{}

HikvisionDevice::~HikvisionDevice()
{}

int HikvisionDevice::login(
	const std::string username, 
	const std::string userpwd, 
	const std::string ip, 
	const unsigned short port /* = 0 */)
{
	int ret{
		!username.empty() && !userpwd.empty() && !ip.empty() && 0 < port ? 
		Error_Code_Success : 
		Error_Code_Invalid_Param };

	if (Error_Code_Success == ret && 
		Error_Code_Success == (ret = initialize()))
	{
		XMem xmem;
		NET_DVR_USER_LOGIN_INFO user{0};
		user.bUseAsynLogin = 0;
		xmem.copy(ip.c_str(), ip.length(), user.sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN);
		user.wPort = port;
		xmem.copy(username.c_str(), username.length(), user.sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN);
		xmem.copy(userpwd.c_str(), userpwd.length(), user.sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN);

		ret = NET_DVR_Login_V40(&user, &deviceInfo);

		if(Error_Code_Success != ret)
		{
			uninitialize();
		}
	}

	return -1 < ret ? ret : Error_Code_Device_Login_Failure;
}

int HikvisionDevice::logout(const int userID/* = -1*/)
{
	int ret{ -1 < userID ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		ret = (NET_DVR_Logout(userID) ? Error_Code_Success : Error_Code_Device_Logout_Failure);

		if (Error_Code_Success == ret)
		{
			ret = uninitialize();
		}
	}

	return ret;
}

const CameraPtrs HikvisionDevice::cameras(const int userID/* = -1*/)
{
	CameraPtrs ptrs;

	for (int i = 0; i != deviceInfo.struDeviceV30.byChanNum; ++i)
	{
		CameraPtr ptr{boost::make_shared<HikvisionCamera>(i)};
		if (ptr)
		{
			ptrs.push_back(ptr);
		}
	}

	LONG groupNo = 0;
	DWORD ret{ 0 };
	NET_DVR_IPPARACFG_V40 ipparamcfg{ 0 };

	if (NET_DVR_GetDVRConfig(userID, NET_DVR_GET_IPPARACFG_V40, 0, &ipparamcfg, sizeof(NET_DVR_IPPARACFG_V40), &ret))
	{
		for (int i = 0; i != ipparamcfg.dwDChanNum; ++i)
		{
			switch (ipparamcfg.struStreamMode[i].byGetStreamType)
			{
			case 0:
			{
				if (ipparamcfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)
				{
					BYTE byIPID{ipparamcfg.struStreamMode[i].uGetStream.struChanInfo.byIPID};
					BYTE byIPIDHigh{ipparamcfg.struStreamMode[i].uGetStream.struChanInfo.byIPIDHigh};
					const int index{ byIPIDHigh * 256 + byIPID - 1 - groupNo * 64 };

					CameraPtr ptr{boost::make_shared<HikvisionCamera>(ipparamcfg.dwStartDChan + i)};
					if (ptr)
					{
						ptr->ipaddr(ipparamcfg.struIPDevInfo[index].struIP.sIpV4);
						ptrs.push_back(ptr);
					}
				}

				break;
			}
			default:
				break;
			}
		}
	}
	
	return std::move(ptrs);
}

int HikvisionDevice::initialize()
{
	int ret{Error_Code_Success};

	if (0 == deviceCounter++)
	{
		ret = (NET_DVR_Init() ? Error_Code_Success : Error_Code_Init_Sdk_Failure);
	}

	return ret;
}

int HikvisionDevice::uninitialize()
{
	int ret{Error_Code_Success};

	if (0 == --deviceCounter)
	{
		ret = (NET_DVR_Cleanup() ? Error_Code_Success : Error_Code_Uninit_Sdk_Failure);
	}

	return ret;
}
