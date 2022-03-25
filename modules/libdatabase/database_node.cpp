#include "boost/make_shared.hpp"
#include "error_code.h"
#include "map/unordered_map.h"
#include "redis/db_redis.h"
#include "database_node.h"
using namespace module::file::database;

using DatabasePtr = boost::shared_ptr<Database>;
static UnorderedMap<const uint32_t, DatabasePtr> dbs;

DatabaseNode::DatabaseNode()
{}

DatabaseNode::~DatabaseNode()
{
	std::vector<DatabasePtr> databases{dbs.values()};
	for (int i = 0; i != databases.size(); ++i)
	{
		databases[i]->disconnect();
	}
	dbs.clear();
}

int DatabaseNode::addConf(const DBModeConf& conf)
{
	int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		if (DatabaseType::DATABASE_TYPE_REDIS == conf.type)
		{
			DatabasePtr db{boost::make_shared<DBRedis>()};
			if (db)
			{
				dbs.add(conf.id, db);
			}

			ret = db ? db->connect(conf.ip, conf.port, conf.passwd) : Error_Code_Bad_New_Object;
		}
		else if (DatabaseType::DATABASE_TYPE_MYSQL == conf.type)
		{

		}
		else
		{
			ret = Error_Code_Operate_Not_Support;
		}
	}
	
	return ret;
}

int DatabaseNode::removeConf(const uint32_t id/* = 0*/)
{
	int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		DatabasePtr db{dbs.at(id)};
		if (db)
		{
			dbs.remove(id);
		}

		ret = db ? db->disconnect() : Error_Code_Object_Not_Exist;
	}

	return ret;
}

int DatabaseNode::write(
	const uint32_t id/* = 0*/, 
	const char* key/* = nullptr*/, 
	const char* value/* = nullptr*/)
{
	int ret{0 < id && key && value ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		DatabasePtr db{dbs.at(id)};
		ret = db ? db->write(key, value) : Error_Code_Object_Not_Exist;
	}
	
	return ret;
}

char* DatabaseNode::read(
	const uint32_t id/* = 0*/, 
	const char* key/* = nullptr*/)
{
	int ret{0 < id && key ? Error_Code_Success : Error_Code_Invalid_Param};
	char* value{nullptr};

	if (Error_Code_Success == ret)
	{
		DatabasePtr db{dbs.at(id)};
		value = (db ? db->read(key) : nullptr);
	}
	
	return value;
}