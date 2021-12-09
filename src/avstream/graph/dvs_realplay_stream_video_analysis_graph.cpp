#include "boost/make_shared.hpp"
#include "error_code.h"
#include "libavstream/av_stream_pin.h"
#include "libavstream/filter/capture/av_stream_capture_filter.h"
#include "libavstream/filter/parser/av_stream_parser_filter.h"
#include "libavstream/filter/callback/av_stream_callback_filter.h"
using namespace module::avstream;
#include "avstream/graph/dvs_realplay_stream_video_analysis_graph.h"
using namespace framework::avstream::graph;

DvsRealplayStreamVideoAnalysisGraph::DvsRealplayStreamVideoAnalysisGraph()
	: RealplayStreamVideoAnalysisGraph()
{}

DvsRealplayStreamVideoAnalysisGraph::~DvsRealplayStreamVideoAnalysisGraph()
{}

int DvsRealplayStreamVideoAnalysisGraph::createNew(void* param/* = nullptr*/)
{
	int ret{Error_Code_Bad_New_Object};
	AVFilterPtr avcapturePtr{boost::make_shared<AVStreamCaptureFilter>()};
	AVFilterPtr avparserPtr{boost::make_shared<AVStreamParserFilter>()};
	AVFilterPtr avcallbackPtr{boost::make_shared<AVStreamCallbackFilter>()};

	if (avcapturePtr && Error_Code_Success == avcapturePtr->createNew(param) &&
		avparserPtr && Error_Code_Success == avparserPtr->createNew() && 
		avcallbackPtr && Error_Code_Success == avcallbackPtr->createNew())
	{
		avfilters.insert(std::make_pair(AVSourceFilterName, avcapturePtr));
		avfilters.insert(std::make_pair(AVSplitterFilterName, avparserPtr));
		avfilters.insert(std::make_pair(AVTargetFilterName, avcallbackPtr));

		ret = RealplayStreamVideoAnalysisGraph::createNew();
	}

	return ret;
}

int DvsRealplayStreamVideoAnalysisGraph::setAVStreamCallback(AVStreamResultCallback callback)
{
	int ret{Error_Code_Object_Not_Exist};
	AVFilterRef avcallbackRef{query(AVTargetFilterName)};

	if(!avcallbackRef.expired())
	{
		using AVStreamCallbackFilterPtr = boost::shared_ptr<AVStreamCallbackFilter>;
		AVStreamCallbackFilterPtr ptr{
			boost::dynamic_pointer_cast<AVStreamCallbackFilter>(avcallbackRef.lock())};
		ptr->setAVStreamCallback(callback);
		ret = Error_Code_Success;
	}

	return ret;
}

int DvsRealplayStreamVideoAnalysisGraph::connectPin()
{
	int ret{Error_Code_Object_Not_Exist};
	AVFilterRef avcaptureRef{query(AVSourceFilterName)};
	AVFilterRef avsplitterRef{query(AVSplitterFilterName)};
	AVFilterRef videoDecoderRef{query(VideoDecoderFilterName)};
	AVFilterRef pictureEncoderRef{query(PictureEncoderFilterName)};
	AVFilterRef avcallbackRef{query(AVTargetFilterName)};

	if (!avcaptureRef.expired() && !avsplitterRef.expired() && 
		!videoDecoderRef.expired() && !pictureEncoderRef.expired() && 
		!avcallbackRef.expired())
	{
		AVPinRef avcaptureOutputPinRef{avcaptureRef.lock()->query(AVOutputPinName)};
		AVPinRef avsplitterInputPinRef{avsplitterRef.lock()->query(AVInputPinName)};
		AVPinRef avsplitterOutputPinRef{avsplitterRef.lock()->query(AVOutputPinName)};
		AVPinRef videoDecoderInputPinRef{videoDecoderRef.lock()->query(AVInputPinName)};
		AVPinRef pictureEncoderOutputPinRef{pictureEncoderRef.lock()->query(AVOutputPinName)};
		AVPinRef avcallbackInputPinRef{avcallbackRef.lock()->query(AVInputPinName)};

		if (!avcaptureOutputPinRef.expired() && !avsplitterInputPinRef.expired() && 
			!avsplitterOutputPinRef.expired() && !videoDecoderInputPinRef.expired() && 
			!pictureEncoderOutputPinRef.expired() && !avcallbackInputPinRef.expired())
		{
			avcaptureOutputPinRef.lock()->connect(avsplitterInputPinRef);
			avsplitterOutputPinRef.lock()->connect(videoDecoderInputPinRef);
			pictureEncoderOutputPinRef.lock()->connect(avcallbackInputPinRef);
			ret = Error_Code_Success;
		}
	}
	
	return ret;
}
