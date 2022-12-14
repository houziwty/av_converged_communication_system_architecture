#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "libavpkt.h"
using namespace module::av::stream;

Libavpkt::Libavpkt(
	const AVMainType mainType /* = AVMainType::AV_MAIN_TYPE_NONE */, 
	const AVSubType subType /* = AVSubType::AV_SUB_TYPE_NONE */, 
	const uint64_t sequence /* = 0 */, 
	const uint64_t timestamp /* = 0 */, 
	const uint32_t w /* = 0 */, 
	const uint32_t h /* = 0 */)
	: avpktMainType{mainType}, avpktSubType{subType}, avpktSequence{sequence}, 
	avpktTimestamp{timestamp}, avpktData{nullptr}, avpktDataBytes{0}, 
	pixelWidth{w}, pixelHeight{h}
{}

Libavpkt::~Libavpkt()
{
	boost::checked_array_delete(reinterpret_cast<uint8_t*>(avpktData));
}

int Libavpkt::input(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		//Auto recover
		boost::checked_array_delete(reinterpret_cast<uint8_t*>(avpktData));
		avpktData = XMem().alloc(data, bytes);
		if (avpktData)
		{
			avpktDataBytes = bytes;
		}
	}
	
	return (avpktData && 0 < avpktDataBytes ? Error_Code_Success : Error_Code_Bad_New_Memory);
}
