#include "boost/make_shared.hpp"
#include "error_code.h"
#include "libmedia/filter/capture/device_sdk/hk_sdk_capture_filter.h"
#include "libmedia/filter/parser/av_packet_parser_filter.h"
#include "libmedia/filter/callback/av_data_callback_filter.h"
#include "graph/hk_cv_analysis_realplay_stream_graph.h"
using namespace framework::media::av;

HKCVAnalysisRealplayStreamGraph::HKCVAnalysisRealplayStreamGraph()
	: CVAnalysisRealplayStreamGraph()
{}

HKCVAnalysisRealplayStreamGraph::~HKCVAnalysisRealplayStreamGraph()
{}

int HKCVAnalysisRealplayStreamGraph::createNew()
{
	int ret{CVAnalysisRealplayStreamGraph::createNew()};

	if (Error_Code_Success == ret)
	{
		FilterPtr hksf{boost::make_shared<HKSdkCaptureFilter>()};
		FilterPtr fpf{boost::make_shared<AVPacketParserFilter>()};
		FilterPtr cbf{boost::make_shared<AVDataCallbackFilter>()};

		if (hksf && Error_Code_Success == hksf->createNew() &&
			ppf && Error_Code_Success == ppf->createNew() && 
			cbf && Error_Code_Success == cbf->createNew())
		{
			filters.insert(std::make_pair(userDefineHKSdkCaptureFilterName, hksf));
			filters.insert(std::make_pair(userDefineAVPacketParserFilterName, ppf));
			filters.insert(std::make_pair(userDefineAVDataCallbackFilterName, cbf));
			ret = connectPin();
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
	int ret{CVAnalysisRealplayStreamGraph::connectPin()};

	if(Error_Code_Success == ret)
	{
		FilterPtr hksf{query(userDefineHKSdkCaptureFilterName)};
		FilterPtr ppf{query(userDefineAVPacketParserFilterName)};
		FilterPtr vdf{query(innerVideoDecoderFilterName)};
		FilterPtr pef{query(innerPictureEncoderFilterName)};
		FilterPtr cbf{query(userDefineAVDataCallbackFilterName)};

		if (hksf && ppf && vdf && pef && cbf)
		{
			PinPtr hksf_out{vdf->query(innerDataOutputPinName)};
			PinPtr ppf_in{ppf->query(innerDataInputPinName)};
			PinPtr ppf_out{ppf->query(innerDataOutputPinName)};
			PinPtr vdf_in{vdf->query(innerDataInputPinName)};
			PinPtr pef_out{pef->query(innerDataOutputPinName)};
			PinPtr cbf_in{cvaf->query(innerDataInputPinName)};

			if (hksf_out && ppf_in && ppf_out && vdf_in && pef_out && cbf_in)
			{
				hksf_out->next(ppf_in);
				ppf_out->next(vdf_in);
				pef_out->next(cbf_in);
				ret = Error_Code_Success;
			}
		}
	}
	
	return ret;
}
