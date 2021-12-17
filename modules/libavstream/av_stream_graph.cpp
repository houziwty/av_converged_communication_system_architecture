#include "error_code.h"
#include "av_stream_graph.h"
using namespace module::avstream;

AVGraph::AVGraph()
{}

AVGraph::~AVGraph()
{
	destroy();
}

AVFilterRef AVGraph::query(const std::string name)
{
	return avfilters.at(name);
}

int AVGraph::createNew(void* /*param = nullptr*/)
{
	if (0 < avfilters.size())
	{
		return Error_Code_Object_Existed;
	}
	
	return connectPin(); 
}

int AVGraph::destroy()
{
	avfilters.clear();
	return avfilters.size() ? Error_Code_Operate_Failure : Error_Code_Success;
}
