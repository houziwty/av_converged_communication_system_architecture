#include "error_code.h"
#include "libmedia/pin.h"
#include "audio_decoder_filter.h"
using namespace module::media::av;

AudioDecoderFilter::AudioDecoderFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

AudioDecoderFilter::~AudioDecoderFilter()
{}

int AudioDecoderFilter::createNew(void* param/* = nullptr*/)
{
	int ret{Filter::createNew(param)};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}
