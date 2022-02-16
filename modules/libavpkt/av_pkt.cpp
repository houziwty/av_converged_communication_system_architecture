#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "av_pkt.h"
using namespace module::av::stream;

AVPkt::AVPkt(
	const AVPktType type/* = AVPktType::AV_PKT_TYPE_NONE*/) 
	: avpktType{type}, avpktData{nullptr}, avpktDataBytes{0}, next{nullptr}
{}

AVPkt::~AVPkt()
{
	boost::checked_array_delete(avpktData);
	boost::checked_delete(next);
}

int AVPkt::input(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		//Auto recover
		boost::checked_array_delete(avpktData);
		avpktData = XMem().alloc(data, bytes);
		ret = (avpktData ? Error_Code_Success : Error_Code_Bad_New_Memory);
	}
	
	return ret;
}

int AVPkt::associate(AVPkt* pkt/* = nullptr*/)
{
	if (avpkt)
	{
		return Error_Code_Object_Existed;
	}
	
	int ret{pkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		avpkt = pkt;
	}
	
	return ret;
}
