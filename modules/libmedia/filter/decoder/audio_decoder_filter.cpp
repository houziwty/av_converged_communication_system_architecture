#include "error_code.h"
#include "libmedia/pin.h"
#include "audio_decoder_filter.h"
using namespace module::media::av;

AudioDecoderFilter::AudioDecoderFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

AudioDecoderFilter::~AudioDecoderFilter()
{}

int AudioDecoderFilter::createNew()
{
	int ret{Filter::createNew()};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

int AudioDecoderFilter::input(FramePtr frame)
{
	return Error_Code_Success;
}

AVProcessorPtr AudioDecoderFilter::createNewProcessor()
{
	return 0;
}
