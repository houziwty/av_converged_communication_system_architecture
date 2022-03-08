#include "error_code.h"
#include "http/http-server-internal.h"
#include "utils/map/unordered_map.h"
#include "http_node.h"
using namespace module::network::http;

using Sessions = UnorderedMap<const uint32_t, struct http_session_t*>;
static Sessions httpSessions;

HttpNode::HttpNode()
{}

HttpNode::~HttpNode()
{
    const std::vector<struct http_session_t*> sessions{httpSessions.values()};
    for (int i = 0; i != sessions.size(); ++i)
    {
        http_session_close(sessions[i]);
    }
}

int HttpNode::addConf(const HTTPModeConf& conf)
{
    int ret{0 < conf.id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        struct http_session_t* session{http_session_create(conf.id)};

        if (session)
        {
            httpSessions.add(conf.id, session);
        }
        else
        {
            ret = Error_Code_Bad_New_Object;
        }
    }
    
    return ret;
}

int HttpNode::removeConf(const uint32_t id/* = 0*/)
{
    int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        struct http_session_t* session{httpSessions.at(id)};

        if (session)
        {
            http_session_close(session);
            httpSessions.remove(id);
        }
        else
        {
            ret = Error_Code_Object_Not_Exist;
        }
    }

    return ret;
}

int HttpNode::input(
	const uint32_t id/* = 0*/, 
    const int32_t error/* = 0*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{0 < id && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		struct http_session_t* session{httpSessions.at(id)};

		if (session)
		{
			ret = http_session_recv(session, error, data, bytes);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}