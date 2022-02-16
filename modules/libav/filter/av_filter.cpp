#include "boost/make_shared.hpp"
#include "error_code.h"
#include "av_pin.h"
#include "av_filter.h"
using namespace module::av::stream;

AVFilter::AVFilter(const AVFilterType type /* = AVFilterType::AV_FILTER_TYPE_NONE */)
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

int AVFilter::createNew(void* param/* = nullptr*/)
{
	if(0 < avpins.size())
	{
		return Error_Code_Object_Existed;
	}

	AVFilterConf* conf{reinterpret_cast<AVFilterConf*>(param)};

	if (AVFilterType::AV_FILTER_TYPE_SOURCE == filterType ||
		AVFilterType::AV_FILTER_TYPE_MEDIUM == filterType)
	{
		AVPinPtr video_out_pin{
			boost::make_shared<AVPin>(*this, AVPinType::PIN_TYPE_OUTPUT)};
		AVPinPtr audio_out_pin{
			boost::make_shared<AVPin>(*this, AVPinType::PIN_TYPE_OUTPUT)};

		//不做位移判断就是要确定标志位值是1、2或3
		//其他值处理
		if (video_out_pin && (1 == conf->flag || 3 == conf->flag))
		{
			avpins.add(av_video_output_pin_name, video_out_pin);
		}
		if (audio_out_pin && (2 == conf->flag || 3 == conf->flag))
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

		//不做位移判断就是要确定标志位值是1、2或3
		//其他值处理
		if (video_in_pin && (1 == conf->flag || 3 == conf->flag))
		{
			avpins.add(av_video_input_pin_name, video_in_pin);
		}
		if (audio_in_pin && (2 == conf->flag || 3 == conf->flag))
		{
			avpins.add(av_audio_input_pin_name, audio_in_pin);
		}
	}

	return 0 < avpins.size() ? Error_Code_Success : Error_Code_Bad_New_Object; 
}

int AVFilter::destroy()
{
	avpins.clear();
	return Error_Code_Success;
}
