#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/map/unordered_map.h"
#include "graph/realplay_stream_render_graph.h"
#include "graph/realplay_stream_video_analysis_graph.h"
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

		if (conf.hwnd)
		{
			graph = boost::make_shared<RealplayStreamRenderGraph>();
		}
		else
		{
			graph = boost::make_shared<RealplayStreamVideoAnalysisGraph>();
		}

		if (graph)
		{
			ret = graph->createNew(conf.hwnd);
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
			ret = graph->destroy();
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
	const uint8_t* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{0 < id && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		AVGraphPtr graph{graphs.at(id)};

		if (graph)
		{
			AVFilterRef source{graph->query(av_data_parser_filter_name)};
			ret = !source.expired() ?source.lock()->input(data) : Error_Code_Operate_Failure;
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}
