#include "error_code.h"
#include "http_session.h"
using namespace module::network::http;

HttpSession::HttpSession() : HttpRequestSplitter()
{}

HttpSession::~HttpSession()
{}

int HttpSession::request(
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
	}

	return ret;
}

// int HttpSession::response(
//     const uint32_t id/* = 0*/, 
//     const int32_t code/* = 0*/, 
//     const char* status/* = nullptr*/, 
//     const HttpResponseHeader* headers/* = nullptr*/, 
//     const uint32_t number/* = 0*/, 
//     const void* data/* = nullptr*/, 
//     const uint64_t bytes/* = 0*/)
// {
//     int ret{0 < id ? Error_Code_Success : Error_Code_Invalid_Param};

// 	return ret;
// }

// int HttpSession::afterHttpReadDataNotification(
//     void* ctx/* = nullptr*/, 
//     http_session_t* session/* = nullptr*/, 
//     const char* method/* = nullptr*/, 
//     const char* path/* = nullptr*/)
// {
//     HttpSession* httpsession{reinterpret_cast<HttpSession*>(ctx)};
//     http_server_set_status_code(session, 200, "OK");
//     http_server_send(session, nullptr, 0, nullptr, 0);

//     const std::size_t headerlen{strlen(session->status_line)}, totallen{headerlen + session->header.len + 2};
//     char* buf = new char[headerlen];
//     memcpy(buf, session->status_line, headerlen);
//     memcpy(buf + headerlen, session->header.ptr, session->header.len);
//     memcpy(buf + headerlen + session->header.len, "\r\n", 2);
//     httpsession->httpnode.onresponse(httpsession->sid, buf, totallen);
//     delete[] buf;
// }
