#include "error_code.h"
#include "http/http-server-internal.h"
#include "map/unordered_map.h"
#include "http_node.h"
using namespace module::network::http;

static struct http_server_t* gHttpServer{nullptr};
using Sessions = UnorderedMap<const uint32_t, struct http_session_t*>;
static Sessions sessions;

int on_http_server_handler(void* param, http_session_t* session, const char* method, const char* path)
{
    int ret{param && session && method && path ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        HttpNode* node{reinterpret_cast<HttpNode*>(param)};
        node->afterPolledReadDataNotification(session->id, method, path);
    }
    
    return ret;
}

int on_http_server_send(void* param, int code, size_t bytes)
{

}

HttpNode::HttpNode()
{
    gHttpServer = http_server_create(nullptr, 0);

    if (gHttpServer)
    {
        http_server_set_handler(gHttpServer, on_http_server_handler, this);
    }

//    http_server_websocket_sethandler(&gHttpServer, nullptr, this);
}

HttpNode::~HttpNode()
{
    std::vector<struct http_session_t*> items{sessions.values()};
    for (int i = 0; i != items.size(); ++i)
    {
        http_session_close(items[i]);
    }

    if (gHttpServer)
    {
        http_server_destroy(gHttpServer);
    }
}

int HttpNode::add(const uint32_t id/* = 0*/)
{
    int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        struct http_session_t* session{http_session_create(id, gHttpServer)};

        if (session)
        {
            sessions.add(id, session);
        }
        else
        {
            ret = Error_Code_Bad_New_Object;
        }
    }
    
    return ret;
}

int HttpNode::remove(const uint32_t id/* = 0*/)
{
    int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

    if (Error_Code_Success == ret)
    {
        struct http_session_t* session{sessions.at(id)};

        if (session)
        {
            http_session_close(session);
            sessions.remove(id);
        }
        else
        {
            ret = Error_Code_Object_Not_Exist;
        }
    }

    return ret;
}

int HttpNode::request(
	const uint32_t id/* = 0*/, 
    const int32_t error/* = 0*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{0 < id && data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		struct http_session_t* session{sessions.at(id)};

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

int HttpNode::response(
    const uint32_t id/* = 0*/, 
    const int32_t code/* = 0*/, 
    const char* status/* = nullptr*/, 
    const HttpResponseHeader* headers/* = nullptr*/, 
    const uint32_t number/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		struct http_session_t* session{sessions.at(id)};

		if (session)
		{
            http_server_set_status_code(session, code, status);
            for (int i = 0; i != number; ++i)
            {
                if (HttpValueType::HTTP_VALUE_TYPE_INTEGER == headers[i].type)
                {
                    http_server_set_header_int(session, headers[i].name, headers[i].i_value);
                }
                else if (HttpValueType::HTTP_VALUE_TYPE_INTEGER == headers[i].type)
                {
                    http_server_set_header(session, headers[i].name, headers[i].c_value);
                }
            }
            
			ret = http_server_send(session, data, bytes, on_http_server_send, this);
		}
		else
		{
			ret = Error_Code_Object_Not_Exist;
		}
	}

	return ret;
}
