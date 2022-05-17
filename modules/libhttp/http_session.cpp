#include "error_code.h"
#include "url/url_parser.h"
using namespace framework::utils::url;
#include "http_session.h"
using namespace module::network::http;

HttpSession::HttpSession() : HttpRequestSplitter()
{}

HttpSession::~HttpSession()
{}

int HttpSession::input(
	const char* data/* = nullptr*/, 
	const std::size_t bytes/* = std::string::npos*/)
{
	int ret{data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		ret = HttpRequestSplitter::input(data, bytes);
	}

	return ret;
}

const std::size_t HttpSession::afterRecvHttpHeaderNotification(
	const char* data/* = nullptr*/, 
	const std::size_t bytes/* = 0*/)
{
    int ret{0 < data && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		UrlParser parser;
		parser.parse(data);
	}

	return ret;
}

void HttpSession::afterRecvHttpContentNotification(
	const char* data/* = nullptr*/, 
	const std::size_t bytes/* = 0*/)
{
}
