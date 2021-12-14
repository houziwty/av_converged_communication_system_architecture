#include "error_code.h"
#include "hikvision_stream.h"
using namespace module::dvs;

HikvisionStream::HikvisionStream()
	: Stream(), streamType{StreamType::STREAM_TYPE_NONE}
{}

HikvisionStream::~HikvisionStream()
{}

int HikvisionStream::open(
	const int userID/* = -1*/, 
	const int index/* = -1*/, 
	const unsigned int type/* = 0*/)
{
	int ret{-1 < userID && -1 < index ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		NET_DVR_PREVIEWINFO previewInfo{0};
		previewInfo.lChannel = index;
		previewInfo.dwStreamType = type;
		previewInfo.byProtoType = 1;
		ret = NET_DVR_RealPlay_V40(
			userID, &previewInfo, &HikvisionStream::livestreamDataCallback, this);
	}

	return -1 < ret ? ret : Error_Code_Stream_Open_Failure;
}

int HikvisionStream::close(const int streamID/* = -1*/)
{
	int ret{-1 < streamID ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		ret = NET_DVR_StopRealPlay(streamID) ? Error_Code_Success : Error_Code_Stream_Close_Failure;
	}

	return ret;
}

// int HikvisionCamera::sendData(
// 	BYTE* pBuffer /* = nullptr */, const DWORD dwBufSize /* = 0 */, const bool head /* = false */)
// {
// 	int e{ pBuffer && 0 < dwBufSize ? eSuccess : eInvalidParameter };

// 	if (eSuccess == e)
// 	{
// 		const int bytes{ framework::data::FixedHeadSize + dwBufSize };
// 		char* buffer{ new(std::nothrow) char[bytes] };

// 		if (buffer)
// 		{
// 			int* flag{ (int*)buffer };
// 			*flag = framework::data::FixedHeadFlag;
// 			int* factoryType{ (int*)(buffer + 4) };
// 			*factoryType = static_cast<int>(framework::data::DataFactory::DATA_FACTORY_HK);
// 			int* dataType{ (int*)(buffer + 8) };
// 			*dataType = static_cast<int>(head ? framework::data::DataType::DATA_TYPE_HEADER : framework::data::DataType::DATA_TYPE_BYTES);
// 			int* frameType{ (int*)(buffer + 12) };
// 			*frameType = -1;
// 			int* frameSize{ (int*)(buffer + 16) };
// 			*frameSize = (int)dwBufSize;
// 			long long* frameSequence{ (long long*)(buffer + 20) };
// 			*frameSequence = -1;
// 			long long* frameTimestamp{ (long long*)(buffer + 28) };
// 			*frameTimestamp = (long long)(Time().tickcount());
// #ifdef WINDOWS
// 			memcpy_s(buffer + framework::data::FixedHeadSize, dwBufSize, pBuffer, dwBufSize);
// #else
// 			memcpy(buffer + framework::data::FixedHeadSize, pBuffer, dwBufSize);
// #endif//WINDOWS

// 			if (tsp)
// 			{
// 				tsp->send(buffer, bytes);
// 			}
// 		}

// 		boost::checked_array_delete(buffer);
// 	}
	
// 	return e;
// }

void HikvisionStream::livestreamDataCallback(
	LONG lPlayHandle, DWORD dwDataType, BYTE* pBuffer, DWORD dwBufSize, void* pUser)
{
	HikvisionStream* hikvisionStream{ reinterpret_cast<HikvisionStream*>(pUser) };

	if (hikvisionStream)
	{
		if (NET_DVR_SYSHEAD == dwDataType || NET_DVR_STREAMDATA == dwDataType)
		{
//			camera->sendData(pBuffer, dwBufSize, NET_DVR_SYSHEAD == dwDataType ? true : false);
		}
		else if (NET_DVR_AUDIOSTREAMDATA == dwDataType)
		{
//			livestream->captureAudioStreamProcess(streamData, dataBytes);
		}
	}
}
