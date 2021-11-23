#include "error_code.h"
#include "libmedia/pin.h"
#include "audio_play_filter.h"
using namespace module::media::av;

AudioPlayFilter::AudioPlayFilter() : MediaTargetFilter()
{}

AudioPlayFilter::~AudioPlayFilter()
{}

int AudioPlayFilter::createNew()
{
	int ret{MediaTargetFilter::createNew()};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

int AudioPlayFilter::input(FramePtr frame)
{
	return Error_Code_Success;
}

AVProcessorPtr AudioPlayFilter::createNewProcessor()
{
	return 0;
}
