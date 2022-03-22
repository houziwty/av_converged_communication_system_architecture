#include "error_code.h"
#include "memory/xstr.h"
using namespace framework::utils::memory;
#include "dahua_device.h"
using namespace module::device::dvs;

uint32_t DahuaDevice::counter = 0;

DahuaDevice::DahuaDevice(
	const DVSModeConf& conf, 
	PolledRealplayDataCallback callback)
	: Device(conf, callback), user{-1}
{}

DahuaDevice::~DahuaDevice()
{
	stop();
}

int DahuaDevice::run()
{
	int ret{0 < modeconf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		if (0 == counter++)
		{
			CLIENT_Init(NULL, 0);
		}
		
		XStr xstr;
		NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY login{0};
		login.dwSize = sizeof(NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY);
		xstr.copy(modeconf.ip, sizeof(modeconf.ip), login.szIP, 64);
		xstr.copy(modeconf.user, sizeof(modeconf.user), login.szUserName, 64);
		xstr.copy(modeconf.passwd, sizeof(modeconf.passwd), login.szPassword, 64);
		login.nPort = modeconf.port;
		login.emSpecCap = EM_LOGIN_SPEC_CAP_TCP;
		NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY out{0};
		out.dwSize = sizeof(NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY);
		user = CLIENT_LoginWithHighLevelSecurity(&login, &out);

		if (-1 < user)
		{
			modeconf.channels = out.stuDeviceInfo.nChanNum;
			for (int i = 0; i != modeconf.channels; ++i)
			{
				NET_IN_REALPLAY_BY_DATA_TYPE in;
				in.dwSize = sizeof(NET_IN_REALPLAY_BY_DATA_TYPE);
				in.nChannelID = i;
				in.hWnd = NULL;
				in.rType = DH_RType_Realplay;
				in.cbRealData = NULL;
				in.emDataType = EM_REAL_DATA_TYPE_GBPS;
				in.dwUser = (LDWORD)this;
				in.szSaveFileName = NULL;
				in.cbRealDataEx = (fRealDataCallBackEx2)&DahuaDevice::livestreamDataCallback;
				in.emAudioType = EM_AUDIO_DATA_TYPE_DEFAULT;
				NET_OUT_REALPLAY_BY_DATA_TYPE out;
				out.dwSize = sizeof(NET_OUT_REALPLAY_BY_DATA_TYPE);

				LLONG sid{ CLIENT_RealPlayByDataType(user, &in, &out, 40) };

				if (sid)
				{
					//通道号从0开始计数
					livestreamIds.add(sid, i);
				}
			}
		}
		
		ret = (-1 < user ? Error_Code_Success : Error_Code_Device_Login_Failure);
	}

	return ret;
}

int DahuaDevice::stop()
{
	int ret{ -1 < user ? Error_Code_Success : Error_Code_Operate_Failure };

	if (Error_Code_Success == ret)
	{
		const std::vector<int32_t> streams{livestreamIds.keies()};
		for (int i = 0; i != streams.size(); ++i)
		{
			CLIENT_StopRealPlayEx(streams[i]);
		}

		ret = CLIENT_Logout(user) ? Error_Code_Success : Error_Code_Device_Logout_Failure;

		if(0 < counter)
		{
			if (0 == --counter)
			{
				CLIENT_Cleanup();
			}
		}
	}

	return ret;
}

void DahuaDevice::livestreamDataCallback(
	LLONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, LONG param, LDWORD dwUser)
{
	DahuaDevice* dvs{ reinterpret_cast<DahuaDevice*>(dwUser) };
	const int32_t channelID{dvs->livestreamIds.at(lRealHandle)};

	if (dvs && -1 < channelID)
	{
		if ((NET_DATA_CALL_BACK_VALUE + EM_REAL_DATA_TYPE_GBPS) == dwDataType)
		{
			if (dvs->polledRealplayDataCallback)
			{
				//平台的通道ID都从1开始
				dvs->polledRealplayDataCallback(dvs->modeconf.id, channelID + 1, dwDataType, pBuffer, dwBufSize);
			}
		}
	}
}
