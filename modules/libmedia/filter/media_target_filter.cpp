#include "error_code.h"
#include "media_target_filter.h"
using namespace module::media::av;

MediaTargetFilter::MediaTargetFilter() 
	: Filter(WorkMode::WORK_MODE_TARGET)
{}

MediaTargetFilter::~MediaTargetFilter()
{}

int MediaTargetFilter::createNew(void* param/* = nullptr*/)
{
	int ret{Filter::createNew(param)};

	if (Error_Code_Success == ret)
	{
		//源过滤器创建时去除输出针脚
		pins.remove(innerDataOutputPinName)
	}
	
	return ret;
}