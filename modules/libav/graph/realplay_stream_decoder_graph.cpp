#include "boost/make_shared.hpp"
#include "defs.h"
#include "error_code.h"
#include "pin/av_pin.h"
#include "filter/parser/av_pkt_parser_filter.h"
#include "filter/decoder/av_frame_decoder_filter.h"
#include "filter/encoder/av_frame_encoder_filter.h"
#include "filter/converter/av_frame_converter_filter.h"
#include "filter/callback/av_data_callback_filter.h"
#include "realplay_stream_decoder_graph.h"
using namespace module::av::stream;

RealplayStreamDecoderGraph::RealplayStreamDecoderGraph()
	: AVGraph()
{}

RealplayStreamDecoderGraph::~RealplayStreamDecoderGraph()
{}

int RealplayStreamDecoderGraph::createNew(void* /*param = nullptr*/)
{
	int ret{Error_Code_Bad_New_Object};
	AVParserModeConf bufferParserConf{1, AVParserType::AV_PARSER_TYPE_BUFFER_PARSER}, 
			psParserConf{1, AVParserType::AV_PARSER_TYPE_PS_PARSER};
	AVFilterPtr bufferParserPtr{boost::make_shared<AVPktParserFilter>(AVFilterType::AV_FILTER_TYPE_SOURCE)};
	AVFilterPtr psParserPtr{boost::make_shared<AVPktParserFilter>(AVFilterType::AV_FILTER_TYPE_MEDIUM)};
	AVFilterPtr videoDecoderPtr{boost::make_shared<AVFrameDecoderFilter>()};
	AVFilterPtr imageConverterPtr{boost::make_shared<AVFrameConverterFilter>()};
	AVFilterPtr dataCallbackPtr{boost::make_shared<AVDataCallbackFilter>()};

	if (bufferParserPtr && Error_Code_Success == bufferParserPtr->createNew(&bufferParserConf) && 
		psParserPtr && Error_Code_Success == psParserPtr->createNew(&psParserConf) && 
		videoDecoderPtr && Error_Code_Success == videoDecoderPtr->createNew() &&
		imageConverterPtr && Error_Code_Success == imageConverterPtr->createNew() && 
		dataCallbackPtr && Error_Code_Success == dataCallbackPtr->createNew())
	{
		avfilters.add(av_buffer_parser_filter_name, bufferParserPtr);
		avfilters.add(av_ps_parser_filter_name, psParserPtr);
		avfilters.add(av_video_decoder_filter_name, videoDecoderPtr);
		avfilters.add(av_image_converter_filter_name, imageConverterPtr);
		avfilters.add(av_data_callback_filter_name, dataCallbackPtr);
		ret = Error_Code_Success;
	}
	
	return Error_Code_Success == ret ? AVGraph::createNew() : ret;
}

int RealplayStreamDecoderGraph::connectPin()
{
	int ret{0 < avfilters.values().size() ? Error_Code_Success : Error_Code_Object_Not_Exist};

	if (Error_Code_Success == ret)
	{
		AVFilterRef bufferParserRef{query(av_buffer_parser_filter_name)};
		AVFilterRef psParserRef{query(av_ps_parser_filter_name)};
		AVFilterRef videoDecoderRef{query(av_video_decoder_filter_name)};
		AVFilterRef imageConverterPtr{query(av_image_converter_filter_name)};
		AVFilterRef dataCallbackPtr{query(av_data_callback_filter_name)};

		if (!bufferParserRef.expired() && !psParserRef.expired() && 
			!videoDecoderRef.expired() && !imageConverterPtr.expired() && 
			!dataCallbackPtr.expired())
		{
			AVPinRef bufferParserOutputPinRef{bufferParserRef.lock()->query(av_video_output_pin_name)};
			AVPinRef psParserInputPinRef{psParserRef.lock()->query(av_video_input_pin_name)};
			AVPinRef psParserOutputPinRef{psParserRef.lock()->query(av_video_output_pin_name)};
			AVPinRef videoFrameDecoderInputPinRef{videoDecoderRef.lock()->query(av_video_input_pin_name)};
			AVPinRef videoFrameDecoderOutputPinRef{videoDecoderRef.lock()->query(av_video_output_pin_name)};
			AVPinRef imageConverterInputPinRef{imageConverterPtr.lock()->query(av_video_input_pin_name)};
			AVPinRef imageConverterOutputPinRef{imageConverterPtr.lock()->query(av_video_output_pin_name)};
			AVPinRef dataCallbackInputPinRef{dataCallbackPtr.lock()->query(av_video_input_pin_name)};

			if (!bufferParserOutputPinRef.expired() && !psParserInputPinRef.expired() && 
				!psParserOutputPinRef.expired() && !videoFrameDecoderInputPinRef.expired() && 
				!videoFrameDecoderOutputPinRef.expired() && !imageConverterInputPinRef.expired() &&
				!imageConverterOutputPinRef.expired() && !dataCallbackInputPinRef.expired())
			{
				bufferParserOutputPinRef.lock()->connect(psParserInputPinRef);
				psParserOutputPinRef.lock()->connect(videoFrameDecoderInputPinRef);
				videoFrameDecoderOutputPinRef.lock()->connect(imageConverterInputPinRef);
				imageConverterOutputPinRef.lock()->connect(dataCallbackInputPinRef);
			}
			else
			{
				ret = Error_Code_Operate_Failure;
			}
		}
	}
	else
	{
		ret = Error_Code_Operate_Failure;
	}
	
	return ret;
}
