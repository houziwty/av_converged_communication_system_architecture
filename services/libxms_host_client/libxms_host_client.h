//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-29
//		Description : 流媒体客户端
//
//		History:
//					1. 2021-11-29 由王科威创建
//

#ifndef LIB_XMS_HOST_CLIENT_H
#define LIB_XMS_HOST_CLIENT_H

#if defined(WINDOWS)
#ifdef USE_XMS_HOST_CLIENT
#define XMS_HOST_CLIENT_EXPORT __declspec(dllimport)
#else
#define XMS_HOST_CLIENT_EXPORT __declspec(dllexport)
#endif//USE_XMS_HOST_CLIENT
#elif defined(__linux__)
#define XMS_HOST_CLIENT_EXPORT
#endif//WINDOWS

#include <string>

class XMS_HOST_CLIENT_EXPORT LibXmsHostClient
{
public:
    LibXmsHostClient(void);
    ~LibXmsHostClient(void);

public:
    //启动
    //@Return : 错误码
    int startXmsHostClient(void);

    //停止
    //@Return : 错误码
    int stopXmsHostClient(void);

    //连接
    //@ip [in] : IP
    //@port [in] : 端口号
    //@Return : 错误码
    //Comment : 异步连接
    int connect(const std::string ip, const unsigned short port = 0);

    //断开
    //@sid [in] : 会话ID
    //@Return : 错误码
    int disconnect(const std::string sid);

    //连接事件通知
    //@sid [out] : 会话ID
    //@e [out] : 错误码
    virtual void fetchXmsHostClientConnectedNotification(const std::string sid, const int e = 0);
};//class LibXmsHostClient

#endif//LIB_XMS_HOST_CLIENT_H