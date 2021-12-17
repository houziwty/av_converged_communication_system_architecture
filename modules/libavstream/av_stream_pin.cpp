#include "error_code.h"
#include "av_stream_filter.h"
#include "av_stream_pin.h"
using namespace module::avstream;

AVPin::AVPin(
	AVFilter& filter, const PinType pt /* = PinType::PIN_TYPE_INPUT */)
	: pinType{pt}, owner{filter}
{}

AVPin::~AVPin()
{}

int AVPin::connect(AVPinRef pin)
{
	if(pin.expired())
	{
		return Error_Code_Invalid_Param;
	}

	if (PinType::PIN_TYPE_OUTPUT == pinType && 
		PinType::PIN_TYPE_INPUT == pin.lock()->getPinType())
	{
		nextInputPin = pin;
	}

	return nextInputPin.expired() ? Error_Code_Operate_Failure : Error_Code_Success;
}

int AVPin::input(void* data/* = nullptr*/)
{
	int ret{ data ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		if (PinType::PIN_TYPE_INPUT == pinType)
		{
			ret = owner.input(data);
		} 
		else if (PinType::PIN_TYPE_OUTPUT == pinType && !nextInputPin.expired())
		{
			ret = nextInputPin.lock()->input(data);
		}
		else
		{
			ret = Error_Code_Operate_Failure;
		}
	}

	return ret;
}
