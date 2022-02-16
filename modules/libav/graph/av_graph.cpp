#include "error_code.h"
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

int AVGraph::createNew(void* /*param = nullptr*/)
{
	return 0 < avfilters.size() ? connectPin() : Error_Code_Object_Not_Exist; 
}

int AVGraph::destroy()
{
	int ret{0 < avfilters.size() ? Error_Code_Success : Error_Code_Object_Not_Exist};

	if (Error_Code_Success == ret)
	{
		std::vector<AVFilterPtr> filters{avfilters.values()};
		for (int i = 0; i != filters.size(); ++i)
		{
			fitlers[i]->destroy();
		}
		avfilters.clear();
	}

	return ret;
}
