#include "error_code.h"
#include "media_target_filter.h"
using namespace module::media::av;

MediaTargetFilter::MediaTargetFilter() 
	: Filter(WorkMode::WORK_MODE_TARGET)
{}

MediaTargetFilter::~MediaTargetFilter()
{}

int MediaTargetFilter::createNew()
{
	int ret{Filter::createNew()};

	if (Error_Code_Success == ret)
	{
		//源过滤器创建时去除输出针脚
		PinPtrs::iterator out{pins.find(innerDataOutputPinName)};
		if(pins.end() != out)
		{
			pins.erase(out);
		}
	}
	
	return ret;
}