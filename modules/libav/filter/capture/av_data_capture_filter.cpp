#include "av_data_capture_filter.h"
using namespace module::av::stream;

AVDataCaptureFilter::AVDataCaptureFilter() 
	: AVFilter(AVFilterType::AV_FILTER_TYPE_SOURCE)
{}

AVDataCaptureFilter::~AVDataCaptureFilter()
{}

int AVDataCaptureFilter::input(const AVPkt* avpkt/* = nullptr*/)
{
	return 0;
}
