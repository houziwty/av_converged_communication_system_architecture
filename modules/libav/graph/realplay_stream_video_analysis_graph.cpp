#include "boost/make_shared.hpp"
#include "error_code.h"
#include "av_stream_pin.h"
#include "filter/decoder/av_stream_decoder_filter.h"
#include "filter/converter/av_stream_converter_filter.h"
#include "filter/analysis/av_stream_analysis_filter.h"
#include "filter/encoder/av_stream_encoder_filter.h"
#include "filter/callback/av_stream_callback_filter.h"
#include "realplay_stream_video_analysis_graph.h"
using namespace module::avstream;

RealplayStreamVideoAnalysisGraph::RealplayStreamVideoAnalysisGraph()
	: AVGraph()
{}

RealplayStreamVideoAnalysisGraph::~RealplayStreamVideoAnalysisGraph()
{}

int RealplayStreamVideoAnalysisGraph::createNew(void* /*param = nullptr*/)
{
	int ret{Error_Code_Bad_New_Object};
	AVFilterPtr videoDecoderPtr{boost::make_shared<AVStreamDecoderFilter>()};
	AVFilterPtr imageConverterPtr{boost::make_shared<AVStreamConverterFilter>()};
	AVFilterPtr videoAnalysisPtr{boost::make_shared<AVStreamAnalysisFilter>()};
	AVFilterPtr pictureEncoderPtr{boost::make_shared<AVStreamEncoderFilter>()};

	if (videoDecoderPtr && Error_Code_Success == videoDecoderPtr->createNew() &&
		imageConverterPtr && Error_Code_Success == imageConverterPtr->createNew() && 
		videoAnalysisPtr && Error_Code_Success == videoAnalysisPtr->createNew() &&
		pictureEncoderPtr && Error_Code_Success == pictureEncoderPtr->createNew())
	{
		avfilters.insert(std::make_pair(VideoDecoderFilterName, videoDecoderPtr));
		avfilters.insert(std::make_pair(ImageConverterFilterName, imageConverterPtr));
		avfilters.insert(std::make_pair(VideoAnalysisFilterName, videoAnalysisPtr));
		avfilters.insert(std::make_pair(PictureEncoderFilterName, pictureEncoderPtr));

		ret = AVGraph::createNew();
	}
	
	return ret;
}

int RealplayStreamVideoAnalysisGraph::connectPin()
{
	int ret{Error_Code_Object_Not_Exist};
	AVFilterRef videoDecoderRef{query(VideoDecoderFilterName)};
	AVFilterRef imageConverterRef{query(ImageConverterFilterName)};
	AVFilterRef videoAnalysisRef{query(VideoAnalysisFilterName)};
	AVFilterRef pictureEncodeRef{query(PictureEncoderFilterName)};

	if (!videoDecoderRef.expired() && !imageConverterRef.expired() && 
		!videoAnalysisRef.expired() && !pictureEncodeRef.expired())
	{
		AVPinRef videoDecoderOutputPinRef{videoDecoderRef.lock()->query(AVOutputPinName)};
		AVPinRef imageConverterInputPinRef{imageConverterRef.lock()->query(AVInputPinName)};
		AVPinRef imageConverterOutputPinRef{imageConverterRef.lock()->query(AVOutputPinName)};
		AVPinRef videoAnalysisInputPinRef{videoAnalysisRef.lock()->query(AVInputPinName)};
		AVPinRef videoAnalysisOutputPinRef{videoAnalysisRef.lock()->query(AVOutputPinName)};
		AVPinRef pictureEncodeInputPinRef{pictureEncodeRef.lock()->query(AVInputPinName)};

		if (!videoDecoderOutputPinRef.expired() && !imageConverterInputPinRef.expired() && 
			!imageConverterOutputPinRef.expired() && !videoAnalysisInputPinRef.expired() && 
			!videoAnalysisOutputPinRef.expired() && !pictureEncodeInputPinRef.expired())
		{
			videoDecoderOutputPinRef.lock()->connect(imageConverterInputPinRef);
			imageConverterOutputPinRef.lock()->connect(videoAnalysisInputPinRef);
			videoAnalysisOutputPinRef.lock()->connect(pictureEncodeInputPinRef);
			ret = Error_Code_Success;
		}
	}
	
	return ret;
}
