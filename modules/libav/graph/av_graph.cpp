#include "error_code.h"
#include "filter/av_filter.h"
#include "av_graph.h"
using namespace module::av::stream;

AVGraph::AVGraph()
{}

AVGraph::~AVGraph()
{
	destroy();
}

AVFilterRef AVGraph::query(const char* name/* = nullptr*/)
{
	return avfilters.at(name);
}

int AVGraph::createNew(const AVModeConf& conf)
{
	return connectPin(); 
}

int AVGraph::destroy(const uint32_t id /* = 0 */)
{
	int ret{0 < id && 0 < avfilters.values().size() ? Error_Code_Success : Error_Code_Object_Not_Exist};

	if (Error_Code_Success == ret)
	{
		std::vector<AVFilterPtr> filters{avfilters.values()};
		for (int i = 0; i != filters.size(); ++i)
		{
			filters[i]->destroy(id);
		}
		avfilters.clear();
	}

	return ret;
}
