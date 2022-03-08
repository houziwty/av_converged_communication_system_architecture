#include "error_code.h"
#include "filter/av_filter.h"
#include "av_pin.h"
using namespace module::av::stream;

AVPin::AVPin(
	AVFilter& filter, const AVPinType type /* = AVPinType::PIN_TYPE_NONE */)
	: pinType{type}, avfilter{filter}
{}

AVPin::~AVPin()
{}

int AVPin::connect(AVPinRef pin)
{
	if(pin.expired())
	{
		return Error_Code_Invalid_Param;
	}

	if (AVPinType::PIN_TYPE_OUTPUT == pinType && 
		AVPinType::PIN_TYPE_INPUT == pin.lock()->type())
	{
		inputPin = pin;
	}

	return inputPin.expired() ? Error_Code_Operate_Failure : Error_Code_Success;
}

int AVPin::input(
	const uint32_t id /* = 0 */, 
	const AVPkt* avpkt /* = nullptr */)
{
	int ret{ avpkt ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		if (AVPinType::PIN_TYPE_INPUT == pinType)
		{
			ret = avfilter.input(id, avpkt);
		} 
		else if (AVPinType::PIN_TYPE_OUTPUT == pinType && !inputPin.expired())
		{
			ret = inputPin.lock()->input(id, avpkt);
		}
		else
		{
			ret = Error_Code_Operate_Failure;
		}
	}

	return ret;
}
