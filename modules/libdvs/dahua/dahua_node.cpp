#include "dhnetsdk.h"
#include "error_code.h"
#include "memory/xstr.h"
using namespace framework::utils::memory;
#include "dahua_node.h"
using namespace module::device::dvs;

uint32_t DahuaNode::counter = 0;

DahuaNode::DahuaNode(
	PolledDataCallback data, PolledExceptionCallback exception)
	: DVSNode(data, exception), enableException{false}
{}

DahuaNode::~DahuaNode()
{}

int64_t DahuaNode::login(
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
			CLIENT_Init((fDisConnect)&DahuaNode::disconnectCallback, (LDWORD)this);
		}

		const std::string sip{ip}, suser{user}, spasswd{passwd};
		NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY login{0};
		login.dwSize = sizeof(NET_IN_LOGIN_WITH_HIGHLEVEL_SECURITY);
		XStr().copy(sip.c_str(), sip.length(), login.szIP, 64);
		XStr().copy(suser.c_str(), suser.length(), login.szUserName, 64);
		XStr().copy(spasswd.c_str(), spasswd.length(), login.szPassword, 64);
		login.nPort = port;
		login.emSpecCap = EM_LOGIN_SPEC_CAP_TCP;
		NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY out{0};
		out.dwSize = sizeof(NET_OUT_LOGIN_WITH_HIGHLEVEL_SECURITY);

		uid = CLIENT_LoginWithHighLevelSecurity(&login, &out);

		if (-1 < uid)
		{
			for (int i = 0; i != out.stuDeviceInfo.nChanNum; ++i)
			{
				chanNums.push_back(i);
			}
		}
	}

	return uid;
}

int DahuaNode::logout(const int64_t uid/* = -1*/)
{
	int ret{-1 < uid ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ret = CLIENT_Logout(uid) ? Error_Code_Success : Error_Code_Device_Logout_Failure;

		if(0 < counter && 0 == --counter)
		{
			CLIENT_Cleanup();
		}
	}

	return ret;
}

int DahuaNode::catchException()
{
	enableException = true;
	CLIENT_SetAutoReconnect((fHaveReConnect)&DahuaNode::reconnectedCallback, (LDWORD)this);
	return Error_Code_Success;
}

int64_t DahuaNode::openRealplay(
	const int64_t uid/* = 0*/, 
	const int32_t channel/* = -1*/)
{
	int64_t sid{-1};

	if (-1 < uid && -1 < channel)
	{
		NET_IN_REALPLAY_BY_DATA_TYPE in;
		in.dwSize = sizeof(NET_IN_REALPLAY_BY_DATA_TYPE);
		in.nChannelID = channel;
		in.hWnd = NULL;
		in.rType = DH_RType_Realplay;
		in.cbRealData = NULL;
		in.emDataType = EM_REAL_DATA_TYPE_GBPS;
		in.dwUser = (LDWORD)this;
		in.szSaveFileName = NULL;
		in.cbRealDataEx = (fRealDataCallBackEx2)&DahuaNode::livestreamDataCallback;
		in.emAudioType = EM_AUDIO_DATA_TYPE_DEFAULT;
		NET_OUT_REALPLAY_BY_DATA_TYPE out;
		out.dwSize = sizeof(NET_OUT_REALPLAY_BY_DATA_TYPE);

		sid = CLIENT_RealPlayByDataType(uid, &in, &out, 40);
	}

	return sid;
}

int DahuaNode::closeRealplay(const int64_t sid/* = 0*/)
{
	int ret{-1 < sid ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ret = CLIENT_StopRealPlayEx(sid) ? Error_Code_Success : Error_Code_Stream_Close_Failure;
	}

	return ret;
}

int DahuaNode::getChanNum(
	const int64_t uid, 
	std::vector<int64_t>& chanNums)
{
	int ret{-1 < uid ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		//NET_IN_GET_CAMERA_STATEINFO in{sizeof(NET_IN_GET_CAMERA_STATEINFO), TRUE};
		//NET_OUT_GET_CAMERA_STATEINFO out{0};
		//
		//if (CLIENT_QueryDevInfo(uid, NET_QUERY_GET_CAMERA_STATE, &in, &out))
		//{
		//	for(int i = 0; i != out.nValidNum; ++i)
		//	{
		//		chanNums.push_back(out.pCameraStateInfo[i].nChannel);
		//	}
		//}
	}

	return ret;
}

void DahuaNode::livestreamDataCallback(
	long lRealHandle, unsigned int dwDataType, unsigned char *pBuffer, unsigned int dwBufSize, int param, long dwUser)
{
	DahuaNode* node{ reinterpret_cast<DahuaNode*>(dwUser) };

	if (node)
	{
		if ((NET_DATA_CALL_BACK_VALUE + EM_REAL_DATA_TYPE_GBPS) == dwDataType)
		{
			if (node->polledDataCallback)
			{
				//平台的通道ID都从1开始
				node->polledDataCallback(node->did, node->streams.at((int64_t)lRealHandle) + 1, dwDataType, pBuffer, dwBufSize);
			}
		}
	}
}

void DahuaNode::disconnectCallback(
	long lLoginID, char *pchDVRIP, int nDVRPort, long dwUser)
{
	DahuaNode* node{ reinterpret_cast<DahuaNode*>(dwUser) };

	if(node && node->enableException)
	{
		node->polledExceptionCallback(node->did, Error_Code_Catch_Device_Exception);
	}
}

void DahuaNode::reconnectedCallback(
	long lLoginID, char *pchDVRIP, int nDVRPort, long dwUser)
{
	DahuaNode* node{ reinterpret_cast<DahuaNode*>(dwUser) };

	if(node && node->enableException)
	{
		node->polledExceptionCallback(node->did, Error_Code_Catch_Device_Exception_Resume);
	}
}
