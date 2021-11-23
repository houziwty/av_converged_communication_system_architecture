#include "boost/make_shared.hpp"
#include "error_code.h"
#include "libmedia/filter/decoder/video_decoder_filter.h"
#include "libmedia/filter/formatter/image_formatter_filter.h"
#include "libmedia/filter/encoder/picture_encoder_filter.h"
#include "libmedia/filter/analysis/cv_analysis_filter.h"
#include "libmedia/pin.h"
#include "libmedia/filter.h"
#include "cv_analysis_realplay_stream_graph.h"
using namespace module::media::av;

CVAnalysisRealplayStreamGraph::CVAnalysisRealplayStreamGraph()
	: Graph()
{}

CVAnalysisRealplayStreamGraph::~CVAnalysisRealplayStreamGraph()
{}

int CVAnalysisRealplayStreamGraph::createNew()
{
	int ret{Graph::createNew()};

	if (Error_Code_Success == ret)
	{
		FilterPtr vdf{boost::make_shared<VideoDecoderFilter>()};
		FilterPtr iff{boost::make_shared<ImageFormatterFilter>()};
		FilterPtr pef{boost::make_shared<PictureEncoderFilter>()};
		FilterPtr cvaf{boost::make_shared<CvAnalysisFilter>()};

		if (vdf && Error_Code_Success == vdf->createNew() &&
			iff && Error_Code_Success == iff->createNew() && 
			pef && Error_Code_Success == pef->createNew() &&
			cvaf && Error_Code_Success == cvaf->createNew())
		{
			filters.insert(std::make_pair(innerVideoDecoderFilterName, vdf));
			filters.insert(std::make_pair(innerImageFormatterFilterName, iff));
			filters.insert(std::make_pair(innerPictureEncoderFilterName, pef));
			filters.insert(std::make_pair(innerCVAnalysisFilterName, cvaf));
		}
		else
		{
			ret = Error_Code_Bad_New_Object;
		}
	}
	
	return ret;
}

int CVAnalysisRealplayStreamGraph::connectPin()
{
	int ret{Error_Code_Object_Not_Exist};
	FilterPtr vdf{query(innerVideoDecoderFilterName)};
	FilterPtr iff{query(innerImageFormatterFilterName)};
	FilterPtr cvaf{query(innerCVAnalysisFilterName)};
	FilterPtr pef{query(innerPictureEncoderFilterName)};

	if (vdf && iff && cvaf && pef)
	{
		PinPtr vdf_out{vdf->query(innerDataOutputPinName)};
		PinPtr iff_out{iff->query(innerDataOutputPinName)};
		PinPtr iff_in{iff->query(innerDataInputPinName)};
		PinPtr cvaf_out{cvaf->query(innerDataOutputPinName)};
		PinPtr cvaf_in{cvaf->query(innerDataInputPinName)};
		PinPtr pef_in{pef->query(innerDataInputPinName)};

		if (vdf_out && iff_in && iff_out && cvaf_out && cvaf_in && pef_in)
		{
			vdf_out->next(iff_in);
			iff_out->next(cvaf_in);
			cvaf_out->next(pef_in);
			ret = Error_Code_Success;
		}
	}
	
	return ret;
}
