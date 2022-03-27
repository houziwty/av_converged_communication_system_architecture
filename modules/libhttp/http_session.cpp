#include <string.h>
#include "error_code.h"
#include "http_session_node.h"
#include "http_session.h"
using namespace module::network::http;

HttpSession::HttpSession(HttpSessionNode& node, const uint32_t id/* = 0*/)
    : httpnode{node}, sid{id}, session{http_session_create()}
{
    if (session)
    {
        session->handler = &HttpSession::afterHttpReadDataNotification;
        session->param = this;
    }
}

HttpSession::~HttpSession()
{
    if (session)
    {
        http_session_close(session);
    }
}

int HttpSession::request(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
        ret = session ? http_session_recv(session, 0, data, bytes) : Error_Code_Operate_Failure;
	}

	return ret;
}

int HttpSession::response(
    const uint32_t id/* = 0*/, 
    const int32_t code/* = 0*/, 
    const char* status/* = nullptr*/, 
    const HttpResponseHeader* headers/* = nullptr*/, 
    const uint32_t number/* = 0*/, 
    const void* data/* = nullptr*/, 
    const uint64_t bytes/* = 0*/)
{
    int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

	// if(Error_Code_Success == ret)
	// {
	// 	struct http_session_t* session{sessions.at(id)};

	// 	if (session)
	// 	{
    //         http_server_set_status_code(session, code, status);
    //         for (int i = 0; i != number; ++i)
    //         {
    //             if (HttpValueType::HTTP_VALUE_TYPE_INTEGER == headers[i].type)
    //             {
    //                 http_server_set_header_int(session, headers[i].name, headers[i].i_value);
    //             }
    //             else if (HttpValueType::HTTP_VALUE_TYPE_INTEGER == headers[i].type)
    //             {
    //                 http_server_set_header(session, headers[i].name, headers[i].c_value);
    //             }
    //         }
            
	// 		ret = http_server_send(session, data, bytes, on_http_server_send, this);
	// 	}
	// 	else
	// 	{
	// 		ret = Error_Code_Object_Not_Exist;
	// 	}
	// }

	return ret;
}

int HttpSession::afterHttpReadDataNotification(
    void* ctx/* = nullptr*/, 
    http_session_t* session/* = nullptr*/, 
    const char* method/* = nullptr*/, 
    const char* path/* = nullptr*/)
{
    HttpSession* httpsession{reinterpret_cast<HttpSession*>(ctx)};
    http_server_set_status_code(session, 200, "OK");
    http_server_send(session, nullptr, 0, nullptr, 0);

    const std::size_t headerlen{strlen(session->status_line)}, totallen{headerlen + session->header.len + 2};
    char* buf = new char[headerlen];
    memcpy(buf, session->status_line, headerlen);
    memcpy(buf + headerlen, session->header.ptr, session->header.len);
    memcpy(buf + headerlen + session->header.len, "\r\n", 2);
    httpsession->httpnode.onresponse(httpsession->sid, buf, totallen);
    delete[] buf;
}
