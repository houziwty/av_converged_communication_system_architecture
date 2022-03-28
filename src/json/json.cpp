#include "boost/algorithm/string.hpp"
#include "boost/foreach.hpp"
#include "boost/property_tree/json_parser.hpp"
#include "error_code.h"
#include "json/json.h"
using namespace framework::utils::data;

Json::Json()
{}

Json::~Json()
{}

int Json::parse(const char* data/* = nullptr*/)
{
    int ret{data ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
		reset();
		std::stringstream ss{data};
		boost::property_tree::json_parser::read_json(ss, root);
    }
    
    return ret;
}

int Json::get(const std::string& path, std::string& out)
{
	int ret{ root.empty() || path.empty() ? Error_Code_Operate_Failure : Error_Code_Success };

	if (Error_Code_Success == ret)
	{
		out.clear();

		try
		{
			out = root.get<std::string>(path);
		}
		catch (boost::property_tree::ptree_bad_path& e)
		{
			ret = Error_Code_Object_Not_Exist;
		}
		catch (boost::property_tree::ptree_bad_data& e)
		{
			ret = Error_Code_Invalid_Param;
		}
	}

	return ret;
}

int Json::get(const std::string& path, std::vector<std::string>& out)
{
	int ret{ root.empty() || path.empty() ? Error_Code_Operate_Failure : Error_Code_Success };

	if (Error_Code_Success == ret)
	{
		out.clear();

		try
		{
			boost::property_tree::ptree node = root.get_child(path);
			BOOST_FOREACH(boost::property_tree::ptree::value_type& value, node)
			{
				BOOST_FOREACH(boost::property_tree::ptree::value_type& subvalue, value.second)
				{
					out.push_back(subvalue.second.data());
				}
			}
		}
		catch (boost::property_tree::ptree_bad_path& e)
		{
			ret = Error_Code_Object_Not_Exist;
		}
		catch (boost::property_tree::ptree_bad_data& e)
		{
			ret = Error_Code_Invalid_Param;
		}
	}

	return ret;
}

int Json::serialize(std::string& out)
{
	int ret{ root.empty() ? Error_Code_Operate_Failure : Error_Code_Success };

	if (Error_Code_Success == ret)
	{
		out.clear();
		std::ostringstream ss;
		boost::property_tree::write_json(ss, root, false);
		out.append(ss.str());
	}

	return ret;
}

int Json::add(const std::string& path, const std::string& value)
{
	int ret{ !path.empty() && !value.empty() ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		boost::property_tree::ptree item;
		boost::property_tree::ptree::assoc_iterator it{ root.find(path) };
		if (it == root.not_found())
		{
			item = root.add(path, value);
		}
		else
		{
			it->second.put_value(value);
		}
	}

	return ret;
}

int Json::add(
	const std::string& name, 
	const Items& items)
{
	int ret{ 0 < items.size() ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		boost::property_tree::ptree kv_item;
		for (int i = 0; i != items.size(); ++i)
		{
			kv_item.put(items[i].first, items[i].second);
		}

		boost::property_tree::ptree::assoc_iterator it{ root.find(name) };
		if (it == root.not_found())
		{
			boost::property_tree::ptree arr_item;
			arr_item.push_back(std::make_pair("", kv_item));
			root.add_child(name, arr_item);
		}
		else
		{
			it->second.push_back(std::make_pair("", kv_item));
		}
	}

	return ret;
}

int Json::remove(const std::string& name)
{
	int ret{ !name.empty() ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		boost::property_tree::ptree::assoc_iterator it{ root.find(name) };
		if (it != root.not_found())
		{
			root.erase(name);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}
