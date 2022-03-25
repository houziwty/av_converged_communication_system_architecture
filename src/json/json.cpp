#include "boost/algorithm/string.hpp"
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
//         unsigned char buf[4096]{ 0 };
//         boost::json::static_resource mr{ buf };
//         boost::json::parse_options opt;
//         opt.allow_comments = true;
//         opt.allow_trailing_commas = true;
//         boost::json::error_code ec;
//         boost::json::parse(data, ec, &mr, opt);
// 
//         if (!ec)
//         {
//             ret = 0;
//         }
    }
    
    return ret;
}

int Json::add(const std::string& path, const std::string& value)
{
	int ret{ !path.empty() && !value.empty() ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
// 		boost::property_tree::ptree sub;
// 
// 		try
// 		{
// 			sub = root.get_child(path);
// 		}
// 		catch (boost::property_tree::ptree_bad_path& e)
// 		{
// 			sub.put(path, value);
// 			root.add_child(path, sub);
// 		}
// 		catch (boost::property_tree::ptree_bad_data& e)
// 		{
// 		}
// 
// 		std::stringstream ss;
// 		boost::property_tree::ptree::
// 		const std::string temp{ boost::json::serialize(obj) };
// 
// 		if (temp.empty())
// 		{
// 		}
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
// 		boost::json::array arr;
// 
// 		for (int i = 0; i != items.size(); ++i)
// 		{
// 			boost::json::object item{ {items[i].first, items[i].second} };
// 			arr.push_back(item);
// 		}
// 		obj.insert(std::make_pair("rus", arr));
// 		const std::string temp{ boost::json::serialize(obj) };
// 
// 		if (temp.empty())
// 		{
// 		}
	}

	return ret;
}
