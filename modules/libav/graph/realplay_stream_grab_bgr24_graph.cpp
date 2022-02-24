#include "boost/make_shared.hpp"
#include "defs.h"
#include "error_code.h"
#include "pin/av_pin.h"
#include "filter/parser/av_buffer_parser_filter.h"
#include "filter/parser/av_frame_parser_filter.h"
#include "filter/decoder/av_frame_decoder_filter.h"
#include "filter/converter/av_frame_converter_filter.h"
#include "realplay_stream_grab_bgr24_graph.h"
using namespace module::av::stream;

RealplayStreamGrabBGR24Graph::RealplayStreamGrabBGR24Graph()
	: AVGraph()
{}

RealplayStreamGrabBGR24Graph::~RealplayStreamGrabBGR24Graph()
{}

int RealplayStreamGrabBGR24Graph::createNew(const AVModeConf& conf)
{
	AVFilterPtr bufferParserPtr{
		boost::make_shared<AVBufferParserFilter>(AVFilterType::AV_FILTER_TYPE_SOURCE)};
	AVFilterPtr frameParserPtr{
		boost::make_shared<AVFrameParserFilter>(AVFilterType::AV_FILTER_TYPE_MEDIUM)};
	AVFilterPtr frmaeDecoderPtr{
		boost::make_shared<AVFrameDecoderFilter>(AVFilterType::AV_FILTER_TYPE_MEDIUM)};
	AVFilterPtr frameConvertPtr{
		boost::make_shared<AVFrameConverterFilter>(AVFilterType::AV_FILTER_TYPE_TARGET)};

	if (bufferParserPtr && Error_Code_Success == bufferParserPtr->createNew(conf) && 
		frameParserPtr && Error_Code_Success == frameParserPtr->createNew(conf) && 
		frmaeDecoderPtr && Error_Code_Success == frmaeDecoderPtr->createNew(conf) && 
		frameConvertPtr && Error_Code_Success == frameConvertPtr->createNew(conf))
	{
		avfilters.add(av_buffer_parser_filter_name, bufferParserPtr);
		avfilters.add(av_frame_parser_filter_name, frameParserPtr);
		avfilters.add(av_frame_decoder_filter_name, frmaeDecoderPtr);
		avfilters.add(av_frame_converter_filter_name, frameConvertPtr);
	}
	
	return 0 < avfilters.values().size() ? AVGraph::createNew(conf) : Error_Code_Bad_New_Object;
}

int RealplayStreamGrabBGR24Graph::connectPin()
{
	int ret{0 < avfilters.values().size() ? Error_Code_Success : Error_Code_Object_Not_Exist};

	if (Error_Code_Success == ret)
	{
		ret = Error_Code_Operate_Failure;
		AVFilterRef bufferParserRef{query(av_buffer_parser_filter_name)};
		AVFilterRef frameParserRef{query(av_frame_parser_filter_name)};
		AVFilterRef frameDecoderRef{query(av_frame_decoder_filter_name)};
		AVFilterRef frameConverterRef{query(av_frame_converter_filter_name)};

		if (!bufferParserRef.expired() && !frameParserRef.expired() && 
			!frameDecoderRef.expired() && !frameConverterRef.expired())
		{
			AVPinRef bufferParserOutputPinRef{bufferParserRef.lock()->query(av_output_pin_name)};
			AVPinRef frameParserInputPinRef{frameParserRef.lock()->query(av_input_pin_name)};
			AVPinRef frameParserOutputPinRef{frameParserRef.lock()->query(av_output_pin_name)};
			AVPinRef frameFrameDecoderInputPinRef{frameDecoderRef.lock()->query(av_input_pin_name)};
			AVPinRef frameFrameDecoderOutputPinRef{frameDecoderRef.lock()->query(av_output_pin_name)};
			AVPinRef frameConverterInputPinRef{frameConverterRef.lock()->query(av_input_pin_name)};

			if (!bufferParserOutputPinRef.expired() && !frameParserInputPinRef.expired() &&
				!frameParserOutputPinRef.expired() && !frameFrameDecoderInputPinRef.expired() && 
				!frameFrameDecoderOutputPinRef.expired() && !frameConverterInputPinRef.expired())
			{
				bufferParserOutputPinRef.lock()->connect(frameParserInputPinRef);
				frameParserOutputPinRef.lock()->connect(frameFrameDecoderInputPinRef);
				frameFrameDecoderOutputPinRef.lock()->connect(frameConverterInputPinRef);
				ret = Error_Code_Success;
			}
		}
	}
	
	return ret;
}
