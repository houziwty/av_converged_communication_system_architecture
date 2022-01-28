#include "boost/make_shared.hpp"
extern "C"
{
#include "HCNetSDK.h"
}
#include "error_code.h"
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "hikvision_device.h"
using namespace module::device::dvs;

uint32_t HikvisionDevice::counter = 0;

HikvisionDevice::HikvisionDevice(const DVSModeConf& conf, PolledDataCallback callback)
	: Device(conf, callback), user{-1}
{}

HikvisionDevice::~HikvisionDevice()
{}

int HikvisionDevice::run()
{
	int ret{0 < modeconf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		if (0 == counter++)
		{
			NET_DVR_Init();
		}
		
		NET_DVR_DEVICEINFO_V40 devinfo{0};
		NET_DVR_USER_LOGIN_INFO logininfo{0};
		logininfo.bUseAsynLogin = 0;
		XMem().copy(modeconf.ip, 128, logininfo.sDeviceAddress, NET_DVR_DEV_ADDRESS_MAX_LEN);
		logininfo.wPort = modeconf.port;
		XMem().copy(modeconf.name, NET_DVR_LOGIN_USERNAME_MAX_LEN, logininfo.sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN);
		XMem().copy(modeconf.passwd, NET_DVR_LOGIN_USERNAME_MAX_LEN, logininfo.sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN);

		user = NET_DVR_Login_V40(&logininfo, &devinfo);
		ret = (-1 < user ? openAllMainStream(&devinfo) : Error_Code_Device_Login_Failure);
	}

	return ret;
}

int HikvisionDevice::stop()
{
	int ret{ -1 < user ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		closeAllMainStream();
		ret = NET_DVR_Logout(user) ? Error_Code_Success : Error_Code_Device_Logout_Failure;

		if (0 == --counter)
		{
			NET_DVR_Cleanup();
		}
	}

	return ret;
}

int HikvisionDevice::openAllMainStream(void* param/* = nullptr*/)
{
	int ret{param ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		NET_DVR_DEVICEINFO_V40* devinfo{
			reinterpret_cast<NET_DVR_DEVICEINFO_V40*>(param)};

		for (int i = 0; i != devinfo->struDeviceV30.byChanNum; ++i)
		{
			indexes.push_back(devinfo->struDeviceV30.byStartChan + i);
		}

		LONG groupNo = 0;
		DWORD e{ 0 };
		NET_DVR_IPPARACFG_V40 ipparamcfg{ 0 };

		if (NET_DVR_GetDVRConfig(user, NET_DVR_GET_IPPARACFG_V40, 0, &ipparamcfg, sizeof(NET_DVR_IPPARACFG_V40), &e))
		{
			for (int i = 0; i != ipparamcfg.dwDChanNum; ++i)
			{
				switch (ipparamcfg.struStreamMode[i].byGetStreamType)
				{
				case 0:
				{
					if (ipparamcfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)
					{
						indexes.push_back(ipparamcfg.dwStartDChan + i);
					}

					break;
				}
				default:
					break;
				}
			}
		}

		for (int i = 0; i != indexes.size(); ++i)
		{
			NET_DVR_PREVIEWINFO preview{0};
			preview.lChannel = indexes[i];
			preview.byProtoType = 1;
			LONG stream{
				NET_DVR_RealPlay_V40(user, &preview, &HikvisionDevice::livestreamDataCallback, this)};

			if (-1 < stream)
			{
				streams.push_back(stream);
			}
		}
	}

	return ret;
}

int HikvisionDevice::closeAllMainStream()
{
	for (int i = 0; i != streams.size(); ++i)
	{
		NET_DVR_StopRealPlay(streams[i]);
	}

	return Error_Code_Success;
}

void HikvisionDevice::livestreamDataCallback(
	int32_t stream, uint32_t type, uint8_t* data, uint32_t bytes, void* ctx)
{
	HikvisionDevice* dvs{ reinterpret_cast<HikvisionDevice*>(ctx) };

	if (dvs)
	{
		if (/*NET_DVR_SYSHEAD == dwDataType || */NET_DVR_STREAMDATA == type)
		{
			if (dvs->polledDataCallback)
			{
				dvs->polledDataCallback(dvs->modeconf.id, stream, type, data, bytes);
			}
		}
		else if (NET_DVR_AUDIOSTREAMDATA == type)
		{
//			livestream->captureAudioStreamProcess(streamData, dataBytes);
		}
	}
}
