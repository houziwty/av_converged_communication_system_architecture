#include "boost/bind/bind.hpp"
using namespace boost::placeholders;
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "map/unordered_map.h"
#include "http_session.h"
#include "libhttp.h"
using namespace module::network::http;

using HttpSessionPtr = boost::shared_ptr<HttpSession>;
using HttpSessionPtrs = UnorderedMap<const uint32_t, HttpSessionPtr>;
static HttpSessionPtrs httpSessionPtrs;

Libhttp::Libhttp()
{}

Libhttp::~Libhttp()
{
    httpSessionPtrs.clear();
}

int Libhttp::addSession(const uint32_t id/* = 0*/)
{
    int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        HttpSessionPtr session{
            boost::make_shared<HttpSession>(
                id, 
                boost::bind(&Libhttp::afterFetchHttpResponseNotification, this, _1, _2, _3), 
                boost::bind(&Libhttp::afterFetchHttpApiEventNotification, this, _1, _2, _3, _4))};

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

int Libhttp::removeSession(const uint32_t id/* = 0*/)
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

int Libhttp::input(
	const uint32_t id/* = 0*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{0 < id && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		HttpSessionPtr session{httpSessionPtrs.at(id)};
        ret = (session ? session->input((const char*)data, bytes) : Error_Code_Object_Not_Exist);
	}

	return ret;
}
