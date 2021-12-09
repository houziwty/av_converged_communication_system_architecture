//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-29
//		Description : 消息接入客户端
//
//		History:
//					1. 2021-11-29 由王科威创建
//

#ifndef SERVICE_LIB_XMQ_HOST_CLIENT_H
#define SERVICE_LIB_XMQ_HOST_CLIENT_H

#if defined(WINDOWS)
#ifdef USE_XMQ_HOST_CLIENT
#define XMQ_HOST_CLIENT_EXPORT __declspec(dllimport)
#else
#define XMQ_HOST_CLIENT_EXPORT __declspec(dllexport)
#endif//USE_DVS_HOST_CLIENT
#elif defined(__linux__)
#define XMQ_HOST_CLIENT_EXPORT
#endif//WINDOWS

#include <string>

class XMQ_HOST_CLIENT_EXPORT LibXmqHostClient
{
public:
    LibXmqHostClient(void);
    ~LibXmqHostClient(void);

public:
    //连接
    //@name [in] : 消息服务名称
    //@ip [in] : IP
    //@port [in] : 端口号
    //@Return : 错误码
    int connect(
        const std::string name, 
        const std::string ip, 
        const unsigned short port = 0);

    //断开
    //@Return : 错误码
    int disconnect(void);
};//class LibXmqHostClient

#endif//SERVICE_LIB_XMQ_HOST_CLIENT_H