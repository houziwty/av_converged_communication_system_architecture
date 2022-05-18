#include "error_code.h"
#include "time/xtime.h"
using namespace framework::utils::time;
#include "url/url_parser.h"
using namespace framework::utils::url;
#include "const/http_const.h"
#include "http_session.h"
using namespace module::network::http;

HttpSession::HttpSession(
	const uint32_t id/* = 0*/, 
	AfterFetchHttpResponseCallback callback/* = nullptr*/) 
	: HttpRequestSplitter(), sid{id}, afterFetchHttpResponseCallback{callback}
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

		if (Error_Code_Success == parser.parse(data))
		{
			//HTTP请求解析成功
			//处理HTTP请求并应答
			const std::string& method{parser.method()};

			if (!method.compare("GET"))
			{
				/* code */
			}
			else if (!method.compare("POST"))
			{
				/* code */
			}
			else if (!method.compare("HEAD"))
			{
				/* code */
			}
			else if (!method.compare("OPTION"))
			{
				/* code */
			}
			else
			{
				response(405);
			}
		}
		else
		{
			//HTTP请求解析失败就产生错误应答
			response(500);
		}
	}

	return ret;
}

void HttpSession::afterRecvHttpContentNotification(
	const char* data/* = nullptr*/, 
	const std::size_t bytes/* = 0*/)
{
}

void HttpSession::response(
	const int code/* = 500*/, 
	const std::multimap<std::string, std::string>& header/* = std::multimap<std::string, std::string>()*/, 
	const char* body/* = nullptr*/)
{
    //消息体默认为空
    std::size_t body_bytes{0};
    if (body)
	{
        //有消息体就获取大小
		const std::string temp{body};
        body_bytes = temp.length();
    }

    std::multimap<std::string, std::string>& headerOut{
		const_cast<std::multimap<std::string, std::string>&>(header)};
	std::multimap<std::string, std::string>::iterator it{headerOut.find("Content-Length")};
	bool close{true};

	if (headerOut.end() == it)
	{
        //http-flv直播是Keep-Alive类型
        close = false;
    } else if (body_bytes >= SIZE_MAX || body_bytes < 0)
	{
        //不固定长度的body，那么发送完body后应该关闭socket，以便浏览器做下载完毕的判断
        close = true;
    }

    headerOut.emplace("Date", XTime().gmt());
    headerOut.emplace("Server", "MediaServer");
    headerOut.emplace("Connection", close ? "close" : "keep-alive");
    if (!close) 
	{
        headerOut.emplace("Keep-Alive", "timeout=30, max=100");
    }

	std::vector<std::string> values;
	auto range{header.equal_range("Origin")};
	for (auto i = range.first; i != range.second; ++i)
    {
        values.push_back(i->second);
    }
    if (0 < values.size())
	{
        //设置跨域
        headerOut.emplace("Access-Control-Allow-Origin", "Origin");
        headerOut.emplace("Access-Control-Allow-Credentials", "true");
    }

    if (headerOut.end() != it && body_bytes >= 0 && body_bytes < SIZE_MAX)
	{
        //文件长度为固定值,且不是http-flv强制设置Content-Length
		headerOut.emplace("Content-Length", std::to_string(body_bytes));
    }

	it = headerOut.find("Content-Type");
    if (0 < body_bytes && headerOut.end() == it)
	{
        //有消息体但没有类型
		//默认使用UTF-8编码
		headerOut.emplace("Content-Type", "text/plain; charset=utf-8");
    }

    //发送HTTP应答
	if (afterFetchHttpResponseCallback)
	{
		std::string resp;
		//消息头
		resp += "HTTP/1.1 ";
		resp += std::to_string(code);
		resp += ' ';
		resp += getHttpStatusMessage(code);
		resp += "\r\n";
		for (auto& it : header)
		{
			resp += it.first;
			resp += ": ";
			resp += it.second;
			resp += "\r\n";
		}
		resp += "\r\n";
		//消息体
		if (body)
		{
			resp += body;
			resp += "\r\n";
		}

		afterFetchHttpResponseCallback(sid, resp.c_str(), resp.length(), close);
	}
}
