#include "error_code.h"
#include "time/xtime.h"
using namespace framework::utils::time;
#include "const/http_const.h"
#include "http_session.h"
#include "http_response_wrapper.h"
using namespace module::network::http;

HttpResponseWrapper::HttpResponseWrapper(HttpSession& session) : httpSession{session}
{}

HttpResponseWrapper::~HttpResponseWrapper()
{}

int HttpResponseWrapper::encode(
    const std::string& protocol, 
    const int code, 
    std::multimap<std::string, std::string>& headers, 
    std::string& content, 
    std::string& rep)
{
    int ret{protocol.empty() ? Error_Code_Invalid_Param : Error_Code_Success};

    if (!ret)
    {
        std::multimap<std::string, std::string>::const_iterator it{headers.find("Content-Length")};
        const bool close{200 > code || (headers.end() != it && 0 < atoi(it->second.c_str())) ? false : true};

        headers.emplace("Date", XTime().gmt());
        headers.emplace("Server", "http_host_server");
        headers.emplace("Connection", close ? "close" : "keep-alive");
        if (!close) 
        {
            headers.emplace("Keep-Alive", "timeout=30, max=100");
        }

        it = headers.find("Origin");
        if (headers.end() != it)
        {
            headers.emplace("Access-Control-Allow-Origin", "Origin");
		    headers.emplace("Access-Control-Allow-Credentials", "true");
        }
        
        //应答行
        rep += (protocol + " " + std::to_string(code) + " " + getHttpStatusMessage(code) + "\r\n");
		//消息头
		for (auto& it : headers)
		{
			rep += (it.first + ":" + it.second + "\r\n");
		}
		rep += "\r\n";
		//消息体
		if (!content.empty())
		{
			rep += (content + "\r\n");
		}
    }

    return ret;
}
