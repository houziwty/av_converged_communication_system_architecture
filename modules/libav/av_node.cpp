#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/map/unordered_map.h"
#ifdef _WINDOWS
#include "graph/realplay_stream_play_graph.h"
#endif//_WINDOWS
#include "graph/realplay_stream_grab_bgr24_graph.h"
#include "filter/av_filter.h"
#include "av_node.h"
using namespace module::av::stream;

using AVGraphPtr = boost::shared_ptr<AVGraph>;
using AVGraphPtrs = UnorderedMap<const uint32_t, AVGraphPtr>;

static AVGraphPtrs graphs;

AVNode::AVNode()
{}

AVNode::~AVNode()
{}

int AVNode::addConf(const AVModeConf& conf)
{
	int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVGraphPtr graph;

		if (AVModeType::AV_MODE_TYPE_GRAB_BRG24 == conf.type)
		{
			graph = boost::make_shared<RealplayStreamGrabBGR24Graph>();
		}
#ifdef _WINDOWS
		else if (AVModeType::AV_MODE_TYPE_STREAM_PLAY == conf.type)
		{
			graph = boost::make_shared<RealplayStreamPlayGraph>();
		}
#endif//_WINDOWS

		if (graph)
		{
			ret = graph->createNew(conf);
			if (Error_Code_Success == ret)
			{
				graphs.add(conf.id, graph);
			}
		}
	}

	return ret;
}

int AVNode::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		AVGraphPtr graph{graphs.at(id)};

		if (graph)
		{
			ret = graph->destroy(id);
			graphs.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int AVNode::input(
	const uint32_t id/* = 0*/, 
	const AVPkt* avpkt/* = nullptr*/)
{
	int ret{0 < id && avpkt ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		AVGraphPtr graph{graphs.at(id)};

		if (graph)
		{
			AVFilterRef source{graph->query(av_buffer_parser_filter_name)};
			ret = (!source.expired() ? source.lock()->input(id, avpkt) : Error_Code_Object_Not_Exist);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}
