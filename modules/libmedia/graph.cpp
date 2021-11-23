#include "error_code.h"
#include "graph.h"
using namespace module::media::av;

Graph::Graph()
{}

Graph::~Graph()
{}

FilterPtr Graph::query(const std::string name)
{
	return filters.at(name);
}

int Graph::createNew()
{
	//清理过滤器
	filters.clear();
	return 0 == filters.size() ? Error_Code_Success : Error_Code_Operate_Failure; 
}
