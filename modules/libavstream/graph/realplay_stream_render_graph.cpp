#include "boost/make_shared.hpp"
#include "error_code.h"
#include "av_stream_pin.h"
#include "filter/decoder/av_stream_decoder_filter.h"
#include "filter/render/av_stream_render_filter.h"
#include "realplay_stream_render_graph.h"
using namespace module::avstream;

RealplayStreamRenderGraph::RealplayStreamRenderGraph()
	: AVGraph()
{}

RealplayStreamRenderGraph::~RealplayStreamRenderGraph()
{}

int RealplayStreamRenderGraph::createNew(void* /*param = nullptr*/)
{
	int ret{Error_Code_Bad_New_Object};
	AVFilterPtr videoDecoderPtr{boost::make_shared<AVStreamDecoderFilter>()};
	AVFilterPtr videoRenderPtr{boost::make_shared<AVStreamRenderFilter>()};
	AVFilterPtr audioDecoderPtr{boost::make_shared<AVStreamDecoderFilter>()};
	AVFilterPtr audioRenderPtr{boost::make_shared<AVStreamRenderFilter>()};

	if (videoDecoderPtr && Error_Code_Success == videoDecoderPtr->createNew() &&
		videoRenderPtr && Error_Code_Success == videoRenderPtr->createNew() && 
		audioDecoderPtr && Error_Code_Success == audioDecoderPtr->createNew() &&
		audioRenderPtr && Error_Code_Success == audioRenderPtr->createNew())
	{
		avfilters.insert(std::make_pair(VideoDecoderFilterName, videoDecoderPtr));
		avfilters.insert(std::make_pair(VideoRenderFilterName, videoRenderPtr));
		avfilters.insert(std::make_pair(AudioDecoderFilterName, audioDecoderPtr));
		avfilters.insert(std::make_pair(AudioPlayerFilterName, audioRenderPtr));

		ret = AVGraph::createNew();
	}
	
	return ret;
}

int RealplayStreamRenderGraph::connectPin()
{
	int ret{Error_Code_Object_Not_Exist};
	AVFilterRef videoDecoderRef{query(VideoDecoderFilterName)};
	AVFilterRef videoRenderRef{query(VideoRenderFilterName)};
	AVFilterRef audioDecoderRef{query(AudioDecoderFilterName)};
	AVFilterRef audioRenderRef{query(AudioPlayerFilterName)};

	if (!videoDecoderRef.expired() && !videoRenderRef.expired() && 
		!audioDecoderRef.expired() && !audioRenderRef.expired())
	{
		AVPinRef videoDecoderOutputPinRef{videoDecoderRef.lock()->query(AVOutputPinName)};
		AVPinRef videoRenderInputPinRef{videoRenderRef.lock()->query(AVInputPinName)};
		AVPinRef audioDecoderOutputPinRef{audioDecoderRef.lock()->query(AVOutputPinName)};
		AVPinRef audioRenderInputPinRef{audioRenderRef.lock()->query(AVInputPinName)};

		if (!videoDecoderOutputPinRef.expired() && !videoRenderInputPinRef.expired() && 
			!audioDecoderOutputPinRef.expired() && !audioRenderInputPinRef.expired())
		{
			videoDecoderOutputPinRef.lock()->connect(videoRenderInputPinRef);
			audioDecoderOutputPinRef.lock()->connect(audioRenderInputPinRef);
			ret = Error_Code_Success;
		}
	}
	
	return ret;
}
