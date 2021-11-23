#include "error_code.h"
#include "media_source_filter.h"
using namespace module::media::av;

MediaSourceFilter::MediaSourceFilter() 
	: Filter(WorkMode::WORK_MODE_SOURCE)
{}

MediaSourceFilter::~MediaSourceFilter()
{}

int MediaSourceFilter::createNew()
{
	int ret{Filter::createNew()};

	if (Error_Code_Success == ret)
	{
		//源过滤器创建时去除输入针脚
		PinPtrs::iterator in{pins.find(innerDataInputPinName)};
		if(pins.end() != in)
		{
			pins.erase(in);
		}
	}
	
	return ret;
}
