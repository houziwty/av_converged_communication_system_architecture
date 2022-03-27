#include "boost/make_shared.hpp"
#include "error_code.h"
#include "map/unordered_map.h"
#include "http_session.h"
#include "http_session_node.h"
using namespace module::network::http;

using HttpSessionPtr = boost::shared_ptr<HttpSession>;
using HttpSessionPtrs = UnorderedMap<const uint32_t, HttpSessionPtr>;
static HttpSessionPtrs httpSessionPtrs;

HttpSessionNode::HttpSessionNode()
{}

HttpSessionNode::~HttpSessionNode()
{
    httpSessionPtrs.clear();
}

int HttpSessionNode::add(const uint32_t id/* = 0*/)
{
    int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        HttpSessionPtr session{boost::make_shared<HttpSession>(*this, id)};

        if (session)
        {
            httpSessionPtrs.add(id, session);
        }
        else
        {
            ret = Error_Code_Bad_New_Object;
        }
    }
    
    return ret;
}

int HttpSessionNode::remove(const uint32_t id/* = 0*/)
{
    int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        HttpSessionPtr session{httpSessionPtrs.at(id)};

        if (session)
        {
            httpSessionPtrs.remove(id);
        }
        else
        {
            ret = Error_Code_Object_Not_Exist;
        }
    }

    return ret;
}

int HttpSessionNode::request(
	const uint32_t id/* = 0*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{0 < id && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		HttpSessionPtr session{httpSessionPtrs.at(id)};

		if (session)
		{
			ret = session->request(data, bytes);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}
