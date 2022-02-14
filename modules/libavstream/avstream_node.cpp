#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/map/unordered_map.h"
#include "graph/realplay_stream_render_graph.h"
#include "graph/realplay_stream_video_analysis_graph.h"
#include "avstream_node.h"
using namespace module::av::stream;

using AVGraphPtr = boost::shared_ptr<AVGraph>;
using AVGraphPtrs = UnorderedMap<const uint32_t, AVGraphPtr>;

static AVGraphPtrs graphs;

AVStreamNode::AVStreamNode()
{}

AVStreamNode::~AVStreamNode()
{}

int AVStreamNode::addConf(const AVModeConf& conf)
{
	int ret{0 < conf.avid ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		AVGraphPtr graph;

		if (hwnd)
		{
			graph = boost::make_shared<RealplayStreamRenderGraph>();
		}
		else
		{
			graph = boost::make_shared<RealplayStreamVideoAnalysisGraph>();
		}

		if (graph)
		{
			graphs.add(conf.avid, graph);
		}
	}

	return ret;
}

int ASIONode::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Operate_Failure};

	if (Error_Code_Success == ret)
	{
		SessionPtr sp{sessions.at(id)};

		if (sp)
		{
			ret = sp->destroy();
			sessions.remove(id);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}

int ASIONode::send(
	const uint32_t id/* = 0*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const char* ip/* = nullptr*/, 
	const uint16_t port/* = 0*/)
{
	int ret{0 < id && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		SessionPtr sp{sessions.at(id)};

		if (sp)
		{
			ret = sp->send(data, bytes, ip, port);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}
