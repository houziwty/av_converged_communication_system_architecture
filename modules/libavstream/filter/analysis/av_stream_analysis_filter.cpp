#include "av_stream_analysis_filter.h"
using namespace module::avstream;

AVStreamAnalysisFilter::AVStreamAnalysisFilter() 
	: AVFilter()
{}

AVStreamAnalysisFilter::~AVStreamAnalysisFilter()
{}

int AVStreamAnalysisFilter::input(void* data/* = nullptr*/)
{
	return 0;
}
