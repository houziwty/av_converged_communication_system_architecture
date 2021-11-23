#include "error_code.h"
#include "libmedia/pin.h"
#include "cv_analysis_filter.h"
using namespace module::media::av;

CvAnalysisFilter::CvAnalysisFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

CvAnalysisFilter::~CvAnalysisFilter()
{}

int CvAnalysisFilter::createNew()
{
	int ret{Filter::createNew()};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}

int CvAnalysisFilter::input(FramePtr frame)
{
	return Error_Code_Success;
}

AVProcessorPtr CvAnalysisFilter::createNewProcessor()
{
	return 0;
}
