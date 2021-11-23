#include "error_code.h"
#include "filter.h"
#include "pin.h"
using namespace module::media::av;

Pin::Pin(
	Filter& parent, 
	const TransportMode tm /* = TransportMode::TRANSPORT_MODE_INPUT */)
	: mode{ tm }, owner{ parent }, nextInputPin{nullptr}
{}

Pin::~Pin()
{}

int Pin::next(PinPtr inpin)
{
	if(nextInputPin)
	{
		return Error_Code_Object_Existed;
	}

	if (TransportMode::TRANSPORT_MODE_INPUT == mode)
	{
		return Error_Code_Operate_Not_Support;
	}

	int ret{ 
		inpin && TransportMode::TRANSPORT_MODE_INPUT == inpin->getTransportMode() ? 
		Error_Code_Success : 
		Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		nextInputPin = inpin;
	}

	return ret;
}

int Pin::input(FramePtr frame)
{
	int ret{ frame ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		if (TransportMode::TRANSPORT_MODE_INPUT == mode)
		{
			ret = owner.input(frame);
		} 
		else if (TransportMode::TRANSPORT_MODE_OUTPUT == mode && nextInputPin)
		{
			ret = nextInputPin->input(frame);
		}
		else
		{
			ret = Error_Code_Operate_Failure;
		}
	}

	return ret;
}
