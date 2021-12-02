#include "error_code.h"
#include "libmedia/pin.h"
#include "audio_play_filter.h"
using namespace module::media::av;

AudioPlayFilter::AudioPlayFilter() : MediaTargetFilter()
{}

AudioPlayFilter::~AudioPlayFilter()
{}

int AudioPlayFilter::createNew(void* param/* = nullptr*/)
{
	int ret{MediaTargetFilter::createNew(param)};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}
