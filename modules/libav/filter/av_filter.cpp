#include "boost/make_shared.hpp"
#include "error_code.h"
#include "pin/av_pin.h"
#include "av_filter.h"
using namespace module::av::stream;

AVFilter::AVFilter(
	const AVFilterType type /* = AVFilterType::AV_FILTER_TYPE_NONE */)
	: filterType{type}
{}

AVFilter::~AVFilter()
{
	destroy();
}

AVPinRef AVFilter::query(const std::string name)
{
	return avpins.at(name);
}

int AVFilter::createNew(const AVModeConf& conf)
{
	if(0 < avpins.values().size())
	{
		return Error_Code_Object_Existed;
	}

	if (AVFilterType::AV_FILTER_TYPE_SOURCE == filterType ||
		AVFilterType::AV_FILTER_TYPE_MEDIUM == filterType)
	{
		AVPinPtr out{
			boost::make_shared<AVPin>(*this, AVPinType::PIN_TYPE_OUTPUT)};
		
		if (out)
		{
			avpins.add(av_output_pin_name, out);
		}
	}

	if (AVFilterType::AV_FILTER_TYPE_TARGET == filterType ||
		AVFilterType::AV_FILTER_TYPE_MEDIUM == filterType)
	{
		AVPinPtr in{
			boost::make_shared<AVPin>(*this, AVPinType::PIN_TYPE_INPUT)};
		
		if (in)
		{
			avpins.add(av_input_pin_name, in);
		}

		if (AVFilterType::AV_FILTER_TYPE_TARGET == filterType)
		{
			avframeDataCallback = conf.callback;
		}
	}

	return 0 < avpins.values().size() ? Error_Code_Success : Error_Code_Bad_New_Object; 
}

int AVFilter::destroy()
{
	avpins.clear();
	return Error_Code_Success;
}

int AVFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	int ret{avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		if (AVFilterType::AV_FILTER_TYPE_TARGET == filterType && avframeDataCallback)
		{
			avframeDataCallback(avpkt);
		}
		else
		{
			AVPinRef out{AVFilter::query(av_output_pin_name)};

			if (!out.expired())
			{
				ret = out.lock()->input(avpkt);
			}
		}
	}
	
	return ret;
}
