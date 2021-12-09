//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-29
//		Description : 设备主机接入客户端SDK
//
//		History:
//					1. 2021-11-29 由王科威创建
//

#ifndef LIB_DVS_HOST_CLIENT_H
#define LIB_DVS_HOST_CLIENT_H

#if defined(WINDOWS)
#ifdef USE_DVS_HOST_CLIENT
#define DVS_HOST_CLIENT_EXPORT __declspec(dllimport)
#else
#define DVS_HOST_CLIENT_EXPORT __declspec(dllexport)
#endif//USE_DVS_HOST_CLIENT
#elif defined(__linux__)
#define DVS_HOST_CLIENT_EXPORT
#endif//WINDOWS

#include <string>

class DVS_HOST_CLIENT_EXPORT DvsHostClient
{
public:
    DvsHostClient(void);
    ~DvsHostClient(void);

public:
    //连接消息服务
    //@name [in] : 消息服务名称
    //@ip [in] : IP
    //@port [in] : 端口号
    //@Return : 错误码
    int connectXMQService(
        const std::string name, 
        const std::string ip, 
        const unsigned short port = 0);

    //断开消息服务连接
    //@Return : 错误码
    int disconnectXMQService(void);
};//class DvsHostClient

#endif//LIB_DVS_HOST_CLIENT_H