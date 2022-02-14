#include "boost/make_shared.hpp"
#include "error_code.h"
#include "av_stream_pin.h"
#include "av_stream_filter.h"
using namespace module::avstream;

AVFilter::AVFilter(const FilterType ft /* = FilterType::FILTER_TYPE_SOURCE */)
	: filterType{ft}
{}

AVFilter::~AVFilter()
{
	destroy();
}

AVPinRef AVFilter::query(const std::string name)
{
	return avpins.at(name);
}

int AVFilter::createNew(void* /*param = nullptr*/)
{
	if(0 < avpins.size())
	{
		return Error_Code_Object_Existed;
	}

	if (FilterType::FILTER_TYPE_SOURCE == filterType ||
		FilterType::FILTER_TYPE_MEDIUM == filterType)
	{
		avpins.insert(
			std::make_pair(
				AVInputPinName, 
				boost::make_shared<AVPin>(*this, PinType::PIN_TYPE_INPUT)));
	}

	if (FilterType::FILTER_TYPE_TARGET == filterType ||
		FilterType::FILTER_TYPE_MEDIUM == filterType)
	{
		avpins.insert(
			std::make_pair(
				AVOutputPinName, 
				boost::make_shared<AVPin>(*this, PinType::PIN_TYPE_OUTPUT)));
	}

	return 0 < avpins.size() ? Error_Code_Success : Error_Code_Bad_New_Object; 
}

int AVFilter::destroy()
{
	avpins.clear();
	return avpins.size() ? Error_Code_Operate_Failure : Error_Code_Success;
}
