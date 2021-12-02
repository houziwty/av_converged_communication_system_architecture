#include "error_code.h"
#include "libmedia/pin.h"
#include "cv_analysis_filter.h"
using namespace module::media::av;

CvAnalysisFilter::CvAnalysisFilter() 
	: Filter(WorkMode::WORK_MODE_MEDIUM)
{}

CvAnalysisFilter::~CvAnalysisFilter()
{}

int CvAnalysisFilter::createNew(void* param/* = nullptr*/)
{
	int ret{Filter::createNew(param)};

	if (Error_Code_Success == ret)
	{
	}
	
	return ret;
}
