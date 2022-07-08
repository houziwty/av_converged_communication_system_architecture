#include "boost/checked_delete.hpp"
#include "boost/compute/detail/sha1.hpp"
#include "boost/format.hpp"
#include "error_code.h"
#include "time/xtime.h"
using namespace framework::utils::time;
#include "url/url_parser.h"
using namespace framework::utils::url;
#include "base64/encoder.h"
using namespace framework::codec::base64;
#include "sha1/encryptor.h"
using namespace framework::encrypt::sha1;
#include "const/http_const.h"
#include "http_session.h"
using namespace module::network::http;

HttpSession::HttpSession(
	const uint32_t id/* = 0*/, 
	AfterFetchHttpResponseCallback rep/* = nullptr*/, 
	AfterFetchHttpRequestCallback req/* = nullptr*/)
	: parser{ *this }, sid{ id }, websocket{ false },
	afterFetchHttpResponseCallback{rep}, afterFetchHttpRequestCallback{req}
{}

HttpSession::~HttpSession()
{}

int HttpSession::input(
	const void* request/* = nullptr*/, 
	const uint32_t bytes/* = 0*/)
{
	int ret{request && 0 < bytes ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		ret = parser.input(request, bytes);
	}

	return ret;
}

void HttpSession::afterParsedHttpRequestNotification(
	std::string& method,
	std::string& url,
	std::string& protocol,
	std::string& content,
	std::unordered_map<std::string, std::string>& headers)
{
	if (!method.compare("GET"))
	{
	}
	else if (!method.compare("POST"))
	{
		afterFetchHttpRequestPost(url, protocol, content, headers);
	}
	else if (!method.compare("OPTIONS"))
	{
		afterFetchHttpRequestOptions();
	}
	else
	{
		sendHttpResponse(405, std::unordered_map<std::string, std::string>(), std::string());
	}
}

void HttpSession::sendHttpResponse(
	const int code,
	std::unordered_map<std::string, std::string>& headers,
	const std::string content) const
{
    //消息体默认为空
    const std::size_t contentlen{content.length()};
	std::unordered_map<std::string, std::string>::iterator it{headers.find("Content-Length")};
	bool close{true};

	 //if (headers.end() == it)
	 //{
  //       //http-flv直播是Keep-Alive类型
  //       close = false;
  //   } 
	 //else if (contentlen >= SIZE_MAX || contentlen < 0)
	 //{
  //       //不固定长度的body，那么发送完body后应该关闭socket，以便浏览器做下载完毕的判断
  //       close = true;
  //   }

	headers.emplace("Date", XTime().gmt());
	headers.emplace("Server", "MediaServer");
	headers.emplace("Connection", close ? "close" : "keep-alive");
    if (!close) 
	{
		headers.emplace("Keep-Alive", "timeout=30, max=100");
    }

	std::vector<std::string> values;
	auto range{headers.equal_range("Origin")};
	for (auto i = range.first; i != range.second; ++i)
    {
        values.push_back(i->second);
    }
    if (0 < values.size())
	{
        //设置跨域
		headers.emplace("Access-Control-Allow-Origin", "Origin");
		headers.emplace("Access-Control-Allow-Credentials", "true");
    }

    if (headers.end() == it && contentlen >= 0 && contentlen < SIZE_MAX)
	{
        //文件长度为固定值,且不是http-flv强制设置Content-Length
		headers.emplace("Content-Length", std::to_string(contentlen));
    }

	it = headers.find("Content-Type");
    if (0 < contentlen && headers.end() == it)
	{
        //有消息体但没有类型
		//默认使用UTF-8编码
		headers.emplace("Content-Type", "text/plain; charset=utf-8");
    }

    //发送HTTP应答
	if (afterFetchHttpResponseCallback)
	{
		std::string resp;
		resp += "HTTP/1.1 ";
		resp += std::to_string(code);
		resp += ' ';
		resp += getHttpStatusMessage(code);
		resp += "\r\n";
		//消息头
		for (auto& it : headers)
		{
			resp += it.first;
			resp += ":";
			resp += it.second;
			resp += "\r\n";
		}
		resp += "\r\n";
		//消息体
		if (!content.empty())
		{
			resp += content;
			resp += "\r\n";
		}

		afterFetchHttpResponseCallback(sid, resp.c_str(), close);
	}
}

void HttpSession::afterFetchHttpRequestOptions() const
{
	std::unordered_map<std::string, std::string> headers;
	headers.emplace("Allow", "GET, POST, OPTIONS");
	headers.emplace("Access-Control-Allow-Origin", "*");
	headers.emplace("Access-Control-Allow-Credentials", "true");
	headers.emplace("Access-Control-Request-Methods", "GET, POST, OPTIONS");
	headers.emplace("Access-Control-Request-Headers", "Accept,Accept-Language,Content-Language,Content-Type");

	sendHttpResponse(200, headers, std::string());
}

void HttpSession::afterFetchHttpRequestPost(
	const std::string& url, 
	const std::string& protocol,
	const std::string& content,
	std::unordered_map<std::string, std::string>& headers)
{
	//尝试升级为Websocket
	websocket = tryUpgradeWebsocket(headers);

	//处理HTTP请求
	if (afterFetchHttpRequestCallback)
	{
		int e{200};
		char* content{nullptr};
		char* content_type{nullptr};
		afterFetchHttpRequestCallback(sid, url.c_str(), e, content, content_type);

		//应答
		std::unordered_map<std::string, std::string> rep_headers;
		if (content_type)
		{
			rep_headers.emplace("Content-Type", content_type);
		}
		rep_headers.emplace("Content-Length", (boost::format("%u") % std::string(content).length()).str());
		sendHttpResponse(e, rep_headers, content);
		boost::checked_array_delete(content);
		boost::checked_array_delete(content_type);
	}
}

bool HttpSession::tryUpgradeWebsocket(
	const std::unordered_map<std::string, std::string>& headers)
{
	bool upgrade{ false };

	if (0 < headers.size())
	{
		try
		{
			const std::string sec_websocket_key{ headers.at("Sec-WebSocket-Key") };
			const std::string sec_websocket_protocol{ headers.at("Sec-WebSocket-Protocol") };
			const std::string sec_websocket_version{ headers.at("Sec-WebSocket-Version") };

			if (!sec_websocket_key.empty() &&
				!sec_websocket_protocol.empty() &&
				!sec_websocket_version.empty())
			{
				upgrade = true;
				//std::multimap<std::string, std::string> headerOut;
				//headerOut.emplace("Upgrade", "websocket");
				//headerOut.emplace("Connection", "Upgrade");
				//headerOut.emplace("Sec-WebSocket-Accept", std::string(Base64Encoder().encode(Sha1Encryptor().encrypt(key[0].c_str()))));
				//headerOut.emplace("Sec-WebSocket-Protocol", proto[0]);
				//headerOut.emplace("Sec-WebSocket-Version", version[0]);
			}
		}
		catch (std::exception&)
		{
		}
	}

	return upgrade;
}
