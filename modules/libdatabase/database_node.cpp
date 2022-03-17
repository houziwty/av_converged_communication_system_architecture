#include "error_code.h"
#include "database.h"
#include "database_node.h"
using namespace module::file::database;

DatabaseNode::DatabaseNode()
{}

DatabaseNode::~DatabaseNode()
{}

int DatabaseNode::addConf(const DBModeConf& conf)
{
	int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};
	return ret;
}

int DatabaseNode::removeConf(const uint32_t id/* = 0*/)
{
	return Error_Code_Success;
}

int DatabaseNode::run(const uint32_t id/* = 0*/)
{
	return Error_Code_Success;
}

int DatabaseNode::stop(const uint32_t id/* = 0*/)
{
	return Error_Code_Success;
}

int DatabaseNode::write(
	const uint32_t id/* = 0*/, 
	const char* key/* = nullptr*/, 
	const char* value/* = nullptr*/)
{
	return Error_Code_Success;
}

const char* DatabaseNode::read(
	const uint32_t id/* = 0*/, 
	const char* key/* = nullptr*/)
{
	return nullptr;
}