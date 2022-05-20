#include "boost/checked_delete.hpp"
#include "boost/compute/detail/sha1.hpp"
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
	AfterFetchHttpResponseCallback resp_cbf/* = nullptr*/, 
	AfterFetchHttpApiEventCallback api_cbf/* = nullptr*/) 
	: HttpRequestSplitter(), sid{id}, websocket{false}, 
	afterFetchHttpResponseCallback{resp_cbf}, 
	afterFetchHttpApiEventCallback{api_cbf}
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

void HttpSession::afterRecvHttpHeaderNotification(
	const char* header, 
	std::size_t& content_length)
{
    int ret{header ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		UrlParser parser;

		//HTTP请求解析
		if (Error_Code_Success == parser.parse(header))
		{
			//获取消息体大小
			std::vector<std::string> content_length_field{parser.value("Content_Length")};
			content_length = (0 < content_length_field.size() ? atoi(content_length_field[0].c_str()) : 0);

			//处理HTTP请求并应答
			const std::string& method{parser.method()};

			if (!method.compare("GET"))
			{
				fetchHttpRequestGet(&parser);
			}
			else if (!method.compare("POST"))
			{
				fetchHttpRequestPost();
			}
			else if (!method.compare("HEAD"))
			{
				fetchHttpRequestHead();
			}
			else if (!method.compare("OPTIONS"))
			{
				fetchHttpRequestOptions();
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
}

void HttpSession::afterRecvHttpContentNotification(
	const char* data/* = nullptr*/, 
	const std::size_t bytes/* = 0*/)
{
	//暂时处理由HTTP GET的消息体
	//GET没有消息体，只处理Websocket的消息
	if (websocket && data && 0 < bytes)
	{
		/* code */
	}	
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

	// if (headerOut.end() == it)
	// {
    //     //http-flv直播是Keep-Alive类型
    //     close = false;
    // } 
	// else if (body_bytes >= SIZE_MAX || body_bytes < 0)
	// {
    //     //不固定长度的body，那么发送完body后应该关闭socket，以便浏览器做下载完毕的判断
    //     close = true;
    // }

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

		afterFetchHttpResponseCallback(sid, resp.c_str(), close);
	}
}

void HttpSession::fetchHttpRequestOptions()
{
	std::multimap<std::string, std::string> header;
    header.emplace("Allow", "GET, POST, HEAD, OPTIONS");
    header.emplace("Access-Control-Allow-Origin", "*");
    header.emplace("Access-Control-Allow-Credentials", "true");
    header.emplace("Access-Control-Request-Methods", "GET, POST, HEAD, OPTIONS");
    header.emplace("Access-Control-Request-Headers", "Accept,Accept-Language,Content-Language,Content-Type");

    response(200, header);
}

void HttpSession::fetchHttpRequestHead()
{
	//暂时全部返回200 OK，因为HTTP GET存在按需生成流的操作，所以不能按照HTTP GET的流程返回
    //如果直接返回404，那么又会导致按需生成流的逻辑失效，所以HTTP HEAD在静态文件或者已存在资源时才有效
    //对于按需生成流的直播场景并不适用
    response(200);
}

void HttpSession::fetchHttpRequestPost()
{
	//暂时不处理POST方法
	response(501);
}

void HttpSession::fetchHttpRequestGet(void* parser/* = nullptr*/)
{
	//尝试升级为Websocket
	tryUpgradeWebsocket(parser);

	//解析HTTP API并执行API事件通知
	UrlParser* _parser{reinterpret_cast<UrlParser*>(parser)};
	const std::string api{_parser->url()};
	if (!api.empty() && afterFetchHttpApiEventCallback)
	{
		int e{200};
		char* body{nullptr};
		afterFetchHttpApiEventCallback(sid, api.c_str(), e, body);
		response(e, std::multimap<std::string, std::string>(), body);
		boost::checked_array_delete(body);
	}
}

void HttpSession::tryUpgradeWebsocket(void* parser/* = nullptr*/)
{
	if (parser)
	{
		UrlParser* _parser{reinterpret_cast<UrlParser*>(parser)};
		const std::vector<std::string> key{_parser->value("Sec-WebSocket-Key")};
		const std::vector<std::string> proto{_parser->value("Sec-WebSocket-Protocol")};
		const std::vector<std::string> version{_parser->value("Sec-WebSocket-Version")};

		//HTTP消息头包含Sec-WebSocket-Key字段就升级Websocket请求
		if (0 < key.size() && 0 < proto.size() && 0 < version.size())
		{
			websocket = true;
			std::multimap<std::string, std::string> headerOut;
			headerOut.emplace("Upgrade", "websocket");
			headerOut.emplace("Connection", "Upgrade");
			headerOut.emplace("Sec-WebSocket-Accept", std::string(Base64Encoder().encode(Sha1Encryptor().encrypt(key[0].c_str()))));
			headerOut.emplace("Sec-WebSocket-Protocol", proto[0]);
			headerOut.emplace("Sec-WebSocket-Version", version[0]);
		}
	}
}
