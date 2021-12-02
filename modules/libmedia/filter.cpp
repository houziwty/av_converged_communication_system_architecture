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

int Filter::createNew(void* param/* = nullptr*/)
{
	//默认的过滤器都会加载输入和输出针脚
	PinPtr in{boost::make_shared<Pin>(*this, TransportMode::TRANSPORT_MODE_INPUT)};
	PinPtr out{boost::make_shared<Pin>(*this, TransportMode::TRANSPORT_MODE_INPUT)};

	if (in && out)
	{
		pins.add(std::make_pair(innerDataInputPinName, in));
		pins.add(std::make_pair(innerDataOutputPinName, out));
	}

	return 0 < pins.size() ? Error_Code_Success : Error_Code_Bad_New_Object; 
}

int Filter::input(void* data/* = nullptr*/)
{
	return avprocessor ? avprocessor->input(data) : Error_Code_Operate_Failure;
}

PinPtr Filter::query(const std::string name)
{
	return pins.at(name);
}
