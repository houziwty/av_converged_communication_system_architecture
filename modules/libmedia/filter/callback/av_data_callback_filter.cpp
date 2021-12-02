#include "libmedia/graph.h"
#include "error_code.h"
#include "av_data_callback_filter.h"
using namespace module::media::av;

AVDataCallbackFilter::AVDataCallbackFilter() 
	: MediaTargetFilter(), graph{nullptr}
{}

AVDataCallbackFilter::~AVDataCallbackFilter()
{}

int AVDataCallbackFilter::createNew(void* param/* = nullptr*/)
{
	int ret{MediaTargetFilter::createNew(param)};

	if (Error_Code_Success == ret)
	{
		graph = reinterpret_cast<Graph*>(param);
	}
	
	return ret;
}

int AVDataCallbackFilter::input(void* data/* = nullptr*/)
{
	int ret{data ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		graph.processDataCompleteNotification(data);
	}

	return ret;
}
