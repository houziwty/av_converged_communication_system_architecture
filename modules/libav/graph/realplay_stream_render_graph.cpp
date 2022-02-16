#include "boost/make_shared.hpp"
#include "error_code.h"
#include "av_stream_pin.h"
#include "filter/parser/av_data_parser_filter.h"
#include "filter/decoder/av_frame_decoder_filter.h"
#include "filter/render/av_stream_render_filter.h"
#include "realplay_stream_render_graph.h"
using namespace module::av::stream;

RealplayStreamRenderGraph::RealplayStreamRenderGraph()
	: AVGraph()
{}

RealplayStreamRenderGraph::~RealplayStreamRenderGraph()
{}

int RealplayStreamRenderGraph::createNew(void* hwnd/* = nullptr*/)
{
	int ret{hwnd ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVFilterConf conf{1, hwnd};
		AVFilterPtr dataParserPtr{boost::make_shared<AVDataParserFilter>()};
		AVFilterPtr videoDecoderPtr{boost::make_shared<AVFrameDecoderFilter>()};
		AVFilterPtr videoRenderPtr{boost::make_shared<AVFrameRenderFilter>()};

		if (dataParserPtr && Error_Code_Success == dataParserPtr->createNew(&conf) && 
			videoDecoderPtr && Error_Code_Success == videoDecoderPtr->createNew(&conf) &&
			videoRenderPtr && Error_Code_Success == videoRenderPtr->createNew(&conf))
		{
			avfilters.insert(std::make_pair(av_data_parser_filter_name, dataParserPtr));
			avfilters.insert(std::make_pair(av_video_decoder_filter_name, videoDecoderPtr));
			avfilters.insert(std::make_pair(av_video_render_filter_name, videoRenderPtr));
		}
		else
		{
			ret = Error_Code_Bad_New_Object;
		}
	}
	
	return Error_Code_Success == ret ? AVGraph::createNew() : ret;
}

int RealplayStreamRenderGraph::connectPin()
{
	int ret{0 < avfilters.size() ? Error_Code_Success : Error_Code_Object_Not_Exist};

	if (Error_Code_Success == ret)
	{
		AVFilterRef dataParserRef{query(av_data_parser_filter_name)};
		AVFilterRef videoDecoderRef{query(av_video_decoder_filter_name)};
		AVFilterRef videoRenderRef{query(av_video_render_filter_name)};

		if (!dataParserRef.expired() && !videoDecoderRef.expired() && !videoRenderRef.expired())
		{
			AVPinRef videoDataParserOutputPinRef{dataParserRef.lock()->query(av_video_output_pin_name)};
			AVPinRef videoFrameDecoderInputPinRef{videoDecoderRef.lock()->query(av_video_input_pin_name)};
			AVPinRef videoFrameDecoderOutputPinRef{videoDecoderRef.lock()->query(av_video_output_pin_name)};
			AVPinRef videoFrameRenderInputPinRef{videoRenderRef.lock()->query(av_video_input_pin_name)};

			if (!videoDataParserOutputPinRef.expired() && !videoFrameDecoderInputPinRef.expired() && 
				!videoFrameDecoderOutputPinRef.expired() && !videoFrameRenderInputPinRef.expired())
			{
				videoDataParserOutputPinRef.lock()->connect(videoFrameDecoderInputPinRef);
				videoFrameDecoderOutputPinRef.lock()->connect(videoFrameRenderInputPinRef);
			}
		}
		else
		{
			ret = Error_Code_Operate_Failure;
		}
	}
	
	return ret;
}
