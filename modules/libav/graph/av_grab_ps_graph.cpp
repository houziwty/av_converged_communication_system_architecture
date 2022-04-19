#include "boost/make_shared.hpp"
#include "error_code.h"
#include "pin/av_pin.h"
#include "filter/parser/av_buffer_parser_filter.h"
#include "av_grab_ps_graph.h"
using namespace module::av::stream;

AVGrabPSGraph::AVGrabPSGraph()
	: AVGraph()
{}

AVGrabPSGraph::~AVGrabPSGraph()
{}

int AVGrabPSGraph::createNew(const AVModeConf& conf)
{
	AVFilterPtr bufferParserPtr{
		boost::make_shared<AVBufferParserFilter>(AVFilterType::AV_FILTER_TYPE_TARGET)};

	if (bufferParserPtr && Error_Code_Success == bufferParserPtr->createNew(conf))
	{
		avfilters.add(av_buffer_parser_filter_name, bufferParserPtr);
	}
	
	return 0 < avfilters.values().size() ? AVGraph::createNew(conf) : Error_Code_Bad_New_Object;
}

int AVGrabPSGraph::connectPin()
{
	return Error_Code_Success;
}
