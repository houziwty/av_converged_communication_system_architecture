#include "error_code.h"
#include "media_source_filter.h"
using namespace module::media::av;

MediaSourceFilter::MediaSourceFilter() 
	: Filter(WorkMode::WORK_MODE_SOURCE)
{}

MediaSourceFilter::~MediaSourceFilter()
{}

int MediaSourceFilter::createNew(void* param/* = nullptr*/)
{
	int ret{Filter::createNew(param)};

	if (Error_Code_Success == ret)
	{
		//源过滤器创建时去除输入针脚
		pins.remove(innerDataInputPinName)
	}
	
	return ret;
}

int MediaSourceFilter::input(void* data/* = nullptr*/)
{
	return Error_Code_Not_Support;
}
