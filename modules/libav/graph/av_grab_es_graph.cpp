#include "boost/make_shared.hpp"
#include "error_code.h"
#include "pin/av_pin.h"
#include "filter/parser/av_buffer_parser_filter.h"
#include "filter/parser/av_frame_parser_filter.h"
#include "av_grab_es_graph.h"
using namespace module::av::stream;

AVGrabESGraph::AVGrabESGraph()
	: AVGraph()
{}

AVGrabESGraph::~AVGrabESGraph()
{}

int AVGrabESGraph::createNew(const AVModeConf& conf)
{
	AVFilterPtr bufferParserPtr{
		boost::make_shared<AVBufferParserFilter>(AVFilterType::AV_FILTER_TYPE_SOURCE)};
	AVFilterPtr frameParserPtr{
		boost::make_shared<AVFrameParserFilter>(AVFilterType::AV_FILTER_TYPE_TARGET)};

	if (bufferParserPtr && Error_Code_Success == bufferParserPtr->createNew(conf) && 
	    frameParserPtr && Error_Code_Success == frameParserPtr->createNew(conf))
	{
		avfilters.add(av_buffer_parser_filter_name, bufferParserPtr);
		avfilters.add(av_frame_parser_filter_name, frameParserPtr);
	}
	
	return 0 < avfilters.values().size() ? AVGraph::createNew(conf) : Error_Code_Bad_New_Object;
}

int AVGrabESGraph::connectPin()
{
	int ret{0 < avfilters.values().size() ? Error_Code_Success : Error_Code_Object_Not_Exist};

	if (Error_Code_Success == ret)
	{
		ret = Error_Code_Operate_Failure;
		AVFilterRef bufferParserRef{query(av_buffer_parser_filter_name)};
		AVFilterRef frameParserRef{query(av_frame_parser_filter_name)};

		if (!bufferParserRef.expired() && !frameParserRef.expired())
		{
			AVPinRef bufferParserOutputPinRef{bufferParserRef.lock()->query(av_output_pin_name)};
			AVPinRef frameParserInputPinRef{frameParserRef.lock()->query(av_input_pin_name)};

			if (!bufferParserOutputPinRef.expired() && !frameParserInputPinRef.expired())
			{
				bufferParserOutputPinRef.lock()->connect(frameParserInputPinRef);
				ret = Error_Code_Success;
			}
		}
	}
	
	return ret;
}
