#include "NetDEVSDK.h"
#include "error_code.h"
#include "libavpkt.h"
using namespace module::av::stream;
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
//		devinfo.dwLoginProto = NETDEV_LOGIN_PROTO_PRIVATE;
		devinfo.dwPort = port;
		XStr().copy(sip.c_str(), sip.length(), devinfo.szIPAddr, NETDEV_LEN_260);
		XStr().copy(suser.c_str(), suser.length(), devinfo.szUserName, NETDEV_LEN_132);
		XStr().copy(spasswd.c_str(), spasswd.length(), devinfo.szPassword, NETDEV_LEN_128);
		NETDEV_SELOG_INFO_S logininfo{0};
		LPVOID loginID{NETDEV_Login_V30(&devinfo, &logininfo)};

		if (loginID)
		{
			NETDEV_VIDEO_CHL_DETAIL_INFO_S channels[256]{0};
			INT32 number{256};
			BOOL ret = NETDEV_QueryVideoChlDetailList(loginID, &number, channels);
			INT32 lastError{NETDEV_GetLastError()};
			if (ret/* || (FALSE == ret &&  NETDEV_E_NEEDMOREDATA == lastError)*/)
			{
				for (int i = 0; i != number; ++i)
				{
					chanNums.push_back(channels[i].dwChannelID);
				}
			}

			uid = (int64_t)loginID;

			AVParserModeConf conf;
			conf.id = 0xff;
			conf.type = AVParserType::AV_PARSER_TYPE_RTP_ES_PARSER;
			conf.cache = 1024 * 1024;
			Libavparser::addConf(conf);
			openRealplay(uid, 1);
		}
	}

	return uid;
}

int UniviewNode::logout(const int64_t uid/* = -1*/)
{
	int ret{0 < uid ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		LPVOID loginID{(LPVOID)uid};
		ret = NETDEV_Logout(loginID) ? Error_Code_Success : Error_Code_Device_Logout_Failure;

		if(0 < counter && 0 == --counter)
		{
			NETDEV_Cleanup();
		}
	}

	return ret;
}

int UniviewNode::catchException()
{
	return 0;
	return NETDEV_SetExceptionCallBack((NETDEV_ExceptionCallBack_PF)&UniviewNode::exceptionCallBack, this) ? 
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
		NETDEV_PREVIEWINFO_S preview{0};
		preview.dwChannelID = channel;
		preview.dwLinkMode = NETDEV_TRANSPROTOCAL_RTPTCP;
		preview.dwStreamMode = NETDEV_STREAM_MODE_VIDEO;
		LPVOID loginID{(LPVOID)uid};
		LPVOID streamID{NETDEV_RealPlay(loginID, &preview, (NETDEV_SOURCE_DATA_CALLBACK_PF)&UniviewNode::livestreamDataCallback, this)};

		if (streamID)
		{
			sid = (int64_t)streamID;
		}
	}

	return sid;
}

int UniviewNode::closeRealplay(const int64_t sid/* = 0*/)
{
	int ret{-1 < sid ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		LPVOID streamID{(LPVOID)sid};
		ret = NETDEV_StopRealPlay(streamID) ? Error_Code_Success : Error_Code_Stream_Close_Failure;
	}

	return ret;
}

int UniviewNode::getChanNum(
	const int64_t uid, 
	std::vector<int64_t>& chanNums)
{
	int ret{-1 < uid ? Error_Code_Success : Error_Code_Invalid_Param};
	return ret;
}

void UniviewNode::livestreamDataCallback(
	void* lRealHandle, const unsigned char *pBuffer, int dwBufSize, int dwMediaDataType, void* pUser)
{
	UniviewNode* node{ reinterpret_cast<UniviewNode*>(pUser) };

	if (node)
	{
		if (NETDEV_MEDIA_DATA_RTP_ES == dwMediaDataType)
		{
			// if (node->polledDataCallback)
			// {
			// 	node->polledDataCallback(node->did, node->streams.at((int64_t)lRealHandle), dwMediaDataType, pBuffer, dwBufSize);
			// }

			Libavpkt pkt;
			pkt.input(pBuffer, dwBufSize);
			node->input(0xff, &pkt);
		}
	}
}

void UniviewNode::exceptionCallBack(
	void* lpUserID, int dwType, void* lpExpHandle, void* lpUserData, void* pstExceptionOutputInfo)
{
	UniviewNode* node{ reinterpret_cast<UniviewNode*>(lpUserData) };

	if (node && node->polledExceptionCallback)
	{
		if (NETDEV_EXCEPTION_EXCHANGE == dwType)
		{
			//Offline
			node->polledExceptionCallback(node->did, node->ip.c_str(), node->sn.c_str(), Error_Code_Catch_Device_Exception);
		}
		// else if (PREVIEW_RECONNECTSUCCESS == dwType)
		// {
		// 	//Online
		// 	node->polledExceptionCallback(node->did, Error_Code_Catch_Device_Exception_Resume);
		// }
	}
}

void UniviewNode::afterParsedDataNotification(
	const uint32_t id/* = 0*/, 
	const void* avpkt/* = nullptr*/)
{

}
