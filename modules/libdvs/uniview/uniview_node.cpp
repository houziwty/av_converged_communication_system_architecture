#include "error_code.h"
#include "memory/xstr.h"
using namespace framework::utils::memory;
#include "uniview_node.h"
using namespace module::device::dvs;

uint32_t UniviewNode::counter = 0;

UniviewNode::UniviewNode(
	PolledDataCallback data, PolledExceptionCallback exception)
	: DVSNode(data, exception)
{}

UniviewNode::~UniviewNode()
{}

int64_t UniviewNode::login(
	const char* ip/* = nullptr*/, 
	const uint16_t port/* = 0*/, 
	const char* user/* = nullptr*/, 
	const char* passwd/* = nullptr*/)
{
	int64_t uid{-1};

	if (ip && 0 < port && user && passwd)
	{
		if (0 == counter++)
		{
			NETDEV_Init();
		}

		const std::string sip{ip}, suser{user}, spasswd{passwd};
		NETDEV_DEVICE_LOGIN_INFO_S devinfo{0};
		devinfo.dwLoginProto = NETDEV_LOGIN_PROTO_PRIVATE;
		devinfo.wPort = port;
		XStr().copy(sip.c_str(), sip.length(), devinfo.szIPAddr, NETDEV_LEN_260);
		XStr().copy(suser.c_str(), suser.length(), logininfo.sUserName, NET_DVR_LOGIN_USERNAME_MAX_LEN);
		XStr().copy(spasswd.c_str(), spasswd.length(), logininfo.sPassword, NET_DVR_LOGIN_PASSWD_MAX_LEN);
		NETDEV_SELOG_INFO_S logininfo{0};

		uid = NETDEV_Login_V30(&logininfo, &devinfo);

		if (-1 < uid)
		{
			for (int i = 0; i != devinfo.struDeviceV30.byChanNum; ++i)
			{
				chanNums.push_back(devinfo.struDeviceV30.byStartChan + i);
			}
		}
	}

	return uid;
}

int UniviewNode::logout(const int64_t uid/* = -1*/)
{
	int ret{-1 < uid ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ret = NET_DVR_Logout(uid) ? Error_Code_Success : Error_Code_Device_Logout_Failure;

		if(0 < counter && 0 == --counter)
		{
			NET_DVR_Cleanup();
		}
	}

	return ret;
}

int UniviewNode::catchException()
{
	return NET_DVR_SetExceptionCallBack_V30(0, nullptr, &HikvisionNode::exceptionCallBack, this) ? 
		Error_Code_Success : 
		Error_Code_Operate_Failure;
}

int64_t UniviewNode::openRealplay(
	const int64_t uid/* = 0*/, 
	const int32_t channel/* = -1*/)
{
	int64_t sid{-1};

	if (-1 < uid && -1 < channel)
	{
		NET_DVR_PREVIEWINFO preview{0};
		preview.lChannel = channel;
		preview.byProtoType = 1;
		sid = NET_DVR_RealPlay_V40(uid, &preview, (REALDATACALLBACK)&HikvisionNode::livestreamDataCallback, this);
	}

	return sid;
}

int UniviewNode::closeRealplay(const int64_t sid/* = 0*/)
{
	int ret{-1 < sid ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ret = NET_DVR_StopRealPlay(sid) ? Error_Code_Success : Error_Code_Stream_Close_Failure;
	}

	return ret;
}

int UniviewNode::getChanNum(
	const int64_t uid, 
	std::vector<int64_t>& chanNums)
{
	int ret{-1 < uid ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		DWORD err{0};
		NET_DVR_DEVICEINFO_V40 devinfo{0};
		NET_DVR_IPPARACFG_V40 ipparamcfg{ 0 };

		if (NET_DVR_GetDVRConfig(uid, NET_DVR_GET_IPPARACFG_V40, 0, &ipparamcfg, sizeof(NET_DVR_IPPARACFG_V40), &err))
		{
			for (int i = 0; i != ipparamcfg.dwDChanNum; ++i)
			{
				switch (ipparamcfg.struStreamMode[i].byGetStreamType)
				{
				case 0:
					{
						if (ipparamcfg.struStreamMode[i].uGetStream.struChanInfo.byEnable)
						{
							chanNums.push_back(ipparamcfg.dwStartDChan + i);
						}

						break;
					}
				default:
					break;
				}
			}
		}
	}

	return ret;
}

void UniviewNode::livestreamDataCallback(
	LONG lRealHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void* pUser)
{
	HikvisionNode* node{ reinterpret_cast<HikvisionNode*>(pUser) };

	if (node)
	{
		if (NET_DVR_STREAMDATA == dwDataType)
		{
			if (node->polledDataCallback)
			{
				node->polledDataCallback(node->did, node->streams.at((int64_t)lRealHandle), dwDataType, pBuffer, dwBufSize);
			}
		}
		else if (NET_DVR_AUDIOSTREAMDATA == dwDataType)
		{
		}
	}
}

void UniviewNode::exceptionCallBack(DWORD dwType, LONG lUserID, LONG lHandle, void *pUser)
{
	HikvisionNode* node{ reinterpret_cast<HikvisionNode*>(pUser) };

	if (node && node->polledExceptionCallback)
	{
		if (EXCEPTION_PREVIEW == dwType || EXCEPTION_RECONNECT == dwType)
		{
			//Offline
			node->polledExceptionCallback(node->did, Error_Code_Catch_Device_Exception);
		}
		else if (PREVIEW_RECONNECTSUCCESS == dwType)
		{
			//Online
			node->polledExceptionCallback(node->did, Error_Code_Catch_Device_Exception_Resume);
		}
	}
}
