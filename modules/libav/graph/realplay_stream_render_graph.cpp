#include "boost/make_shared.hpp"
#include "defs.h"
#include "error_code.h"
#include "pin/av_pin.h"
#include "filter/parser/av_pkt_parser_filter.h"
#include "filter/decoder/av_frame_decoder_filter.h"
#include "filter/render/av_frame_render_filter.h"
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
		AVParserModeConf bufferParserConf{1, AVParserType::AV_PARSER_TYPE_BUFFER_PARSER}, 
			psParserConf{2, AVParserType::AV_PARSER_TYPE_PS_PARSER};
		AVFilterPtr bufferParserPtr{boost::make_shared<AVPktParserFilter>(AVFilterType::AV_FILTER_TYPE_SOURCE)};
		AVFilterPtr psParserPtr{boost::make_shared<AVPktParserFilter>(AVFilterType::AV_FILTER_TYPE_MEDIUM)};
		AVFilterPtr videoDecoderPtr{boost::make_shared<AVFrameDecoderFilter>()};
		AVFilterPtr videoRenderPtr{boost::make_shared<AVFrameRenderFilter>()};

		if (bufferParserPtr && Error_Code_Success == bufferParserPtr->createNew(&bufferParserConf) && 
			psParserPtr && Error_Code_Success == psParserPtr->createNew(&psParserConf) && 
			videoDecoderPtr && Error_Code_Success == videoDecoderPtr->createNew(&conf) &&
			videoRenderPtr && Error_Code_Success == videoRenderPtr->createNew(&conf))
		{
			avfilters.add(av_buffer_parser_filter_name, bufferParserPtr);
			avfilters.add(av_ps_parser_filter_name, psParserPtr);
			avfilters.add(av_video_decoder_filter_name, videoDecoderPtr);
			avfilters.add(av_video_render_filter_name, videoRenderPtr);
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
	int ret{0 < avfilters.values().size() ? Error_Code_Success : Error_Code_Object_Not_Exist};

	if (Error_Code_Success == ret)
	{
		AVFilterRef bufferParserRef{query(av_buffer_parser_filter_name)};
		AVFilterRef psParserRef{query(av_ps_parser_filter_name)};
		AVFilterRef videoDecoderRef{query(av_video_decoder_filter_name)};
		AVFilterRef videoRenderRef{query(av_video_render_filter_name)};

		if (!bufferParserRef.expired() && !psParserRef.expired() &&!videoDecoderRef.expired() && !videoRenderRef.expired())
		{
			AVPinRef bufferParserOutputPinRef{bufferParserRef.lock()->query(av_video_output_pin_name)};
			AVPinRef psParserInputPinRef{psParserRef.lock()->query(av_video_input_pin_name)};
			AVPinRef psParserOutputPinRef{psParserRef.lock()->query(av_video_output_pin_name)};
			AVPinRef videoFrameDecoderInputPinRef{videoDecoderRef.lock()->query(av_video_input_pin_name)};
			AVPinRef videoFrameDecoderOutputPinRef{videoDecoderRef.lock()->query(av_video_output_pin_name)};
			AVPinRef videoFrameRenderInputPinRef{videoRenderRef.lock()->query(av_video_input_pin_name)};

			if (!bufferParserOutputPinRef.expired() && !psParserInputPinRef.expired() &&
				!psParserOutputPinRef.expired() && !videoFrameDecoderInputPinRef.expired() && 
				!videoFrameDecoderOutputPinRef.expired() && !videoFrameRenderInputPinRef.expired())
			{
				bufferParserOutputPinRef.lock()->connect(psParserInputPinRef);
				psParserOutputPinRef.lock()->connect(videoFrameDecoderInputPinRef);
				videoFrameDecoderOutputPinRef.lock()->connect(videoFrameRenderInputPinRef);
			}
			else
			{
				ret = Error_Code_Operate_Failure;
			}
		}
		else
		{
			ret = Error_Code_Operate_Failure;
		}
	}
	
	return ret;
}
