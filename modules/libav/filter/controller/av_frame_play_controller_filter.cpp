#include "error_code.h"
#include "av_frame_play_controller_filter.h"
using namespace module::av::stream;

AVFramePlayControllerFilter::AVFramePlayControllerFilter() 
	: AVBufferParserFilter(AVFilterType::AV_FILTER_TYPE_SOURCE), 
	playControlType(PlayControlType::PLAY_CONTROL_TYPE_PLAY), param{0}
{}

AVFramePlayControllerFilter::~AVFramePlayControllerFilter()
{}

int AVFramePlayControllerFilter::input(
	const uint32_t id /* = 0 */, 
	const void* avpkt /* = nullptr */)
{
	int ret{AVBufferParserFilter::input(id, avpkt)};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

int AVFramePlayControllerFilter::control(
	const uint32_t id/* = 0*/,
	const PlayControlType type/* = PlayControlType::PLAY_CONTROL_TYPE_NONE*/,
	const int32_t param/* = 0*/)
{
	playControlType = type;
	this->param = param;
	return Error_Code_Success;
}
