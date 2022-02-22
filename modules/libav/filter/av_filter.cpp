#include "boost/make_shared.hpp"
#include "error_code.h"
#include "defs.h"
#include "pin/av_pin.h"
#include "av_filter.h"
using namespace module::av::stream;

AVFilter::AVFilter(
	const AVFilterType type /* = AVFilterType::AV_FILTER_TYPE_NONE */, 
	const AVFilterConf conf /*= AVFilterConf::AV_FILTER_CONF_NONE*/)
	: filterType{type}, filterConf{conf}
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
		AVPinPtr video_out_pin{
			boost::make_shared<AVPin>(*this, AVPinType::PIN_TYPE_OUTPUT)};
		AVPinPtr audio_out_pin{
			boost::make_shared<AVPin>(*this, AVPinType::PIN_TYPE_OUTPUT)};

		//动态针脚实例创建
		if (video_out_pin && 
			(AVFilterConf::AV_FILTER_CONF_AV == filterConf || AVFilterConf::AV_FILTER_CONF_VIDEO == filterConf))
		{
			avpins.add(av_video_output_pin_name, video_out_pin);
		}
		
		if (audio_out_pin && 
			(AVFilterConf::AV_FILTER_CONF_AV == filterConf || AVFilterConf::AV_FILTER_CONF_AUDIO == filterConf))
		{
			avpins.add(av_audio_output_pin_name, audio_out_pin);
		}
	}

	if (AVFilterType::AV_FILTER_TYPE_TARGET == filterType ||
		AVFilterType::AV_FILTER_TYPE_MEDIUM == filterType)
	{
		AVPinPtr video_in_pin{
			boost::make_shared<AVPin>(*this, AVPinType::PIN_TYPE_INPUT)};
		AVPinPtr audio_in_pin{
			boost::make_shared<AVPin>(*this, AVPinType::PIN_TYPE_INPUT)};

		//动态针脚实例创建
		if (video_in_pin && 
			(AVFilterConf::AV_FILTER_CONF_AV == filterConf || AVFilterConf::AV_FILTER_CONF_VIDEO == filterConf))
		{
			avpins.add(av_video_input_pin_name, video_in_pin);
		}
		if (audio_in_pin && 
			(AVFilterConf::AV_FILTER_CONF_AV == filterConf || AVFilterConf::AV_FILTER_CONF_AUDIO == filterConf))
		{
			avpins.add(av_audio_input_pin_name, audio_in_pin);
		}
	}

	return 0 < avpins.values().size() ? Error_Code_Success : Error_Code_Bad_New_Object; 
}

int AVFilter::destroy()
{
	avpins.clear();
	return Error_Code_Success;
}
