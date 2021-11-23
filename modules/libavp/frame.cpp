#include <new>
#include "boost/checked_delete.hpp"
#include "utils/memory/xmemcpy.h"
using namespace framework::utils::memory;
#include "error_code.h"
#include "frame.h"
using namespace module::media::av;

Frame::Frame(
	const MainType mt/* = MainType::MAIN_TYPE_NONE*/, 
	const SubType st/* = SubType::SUB_TYPE_NONE*/)
	: mainType{mt}, subType{st}, in{nullptr}, insize{0}
{}

Frame::~Frame()
{
	boost::checked_array_delete(in);
}

int Frame::input(
	void* data/* = nullptr*/, 
	const unsigned long long bytes/* = 1*/)
{
	if(in)
	{
		return Error_Code_Object_Existed;
	}

	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		in = new(std::nothrow) unsigned char[bytes];
		if(in)
		{
			xmemcpy(data, in, bytes);
			insize = bytes;
		}
	}

	return in ? Error_Code_Success : Error_Code_Bad_New_Memory;
}
