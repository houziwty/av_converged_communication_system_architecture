#include "boost/make_shared.hpp"
#include "libavp/avp.h"
#include "error_code.h"
#include "pin.h"
#include "filter.h"
using namespace module::media::av;

Filter::Filter(const WorkMode wm /* = WorkMode::WORK_MODE_SOURCE */)
	: mode{ wm }
{}

Filter::~Filter()
{}

int Filter::createNew()
{
	//清理过滤器
	pins.clear();

	//默认的过滤器都会加载输入和输出针脚
	PinPtr in{boost::make_shared<Pin>(*this, TransportMode::TRANSPORT_MODE_INPUT)};
	PinPtr out{boost::make_shared<Pin>(*this, TransportMode::TRANSPORT_MODE_INPUT)};

	if (in && out)
	{
		pins.insert(std::make_pair(innerDataInputPinName, in));
		pins.insert(std::make_pair(innerDataOutputPinName, out));
	}

	return 0 < pins.size() ? Error_Code_Success : Error_Code_Bad_New_Object; 
}

int Filter::input(FramePtr frame)
{
	int ret{
		WorkMode::WORK_MODE_SOURCE == getWorkMode() ? 
		Error_Code_Method_Not_Support : 
		Error_Code_Success};

	if(Error_Code_Success == ret)
	{
		if(!avprocessor)
		{
			avprocessor = createNewProcessor();
		}

		ret = (avprocessor ? avprocessor->input(frame) : Error_Code_Bad_New_Object);
	}

	return ret;
}

PinPtr Filter::query(const std::string name)
{
	return pins.at(name);
}
