#include "boost/checked_delete.hpp"
#include "error_code.h"
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "data.h"
using namespace module::common::data;

Data::Data() : data{nullptr}, bytes{0}
{}

Data::~Data()
{}

int Data::input(
	const uint8_t* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		//Auto recover
		boost::checked_array_delete(data);
		this->data = XMem().alloc(data, bytes);
		ret = (this->data ? Error_Code_Success : Error_Code_Bad_New_Memory);
	}
	
	return ret;
}
