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
    virtual ~LibXmsHostClient(void);

public:
    //启动
    //@Return : 错误码
    virtual int start(void);

    //停止
    //@Return : 错误码
    virtual int stop(void);

    //连接
    //@ip [in] : IP地址
    //@port [in] : 端口号
    //@Return : 错误码
    //Comment : 异步连接
    int connect(const std::string ip, const unsigned short port = 0);

    //断开
    //@sid [in] : 会话ID
    //@Return : 错误码
    int disconnect(const std::string sid);

    //发送
    //@sid [in] : 会话ID
    //@data [in] : 数据
    //@bytes [in] : 大小
    //@Return : 错误码
    int send(
        const std::string sid, 
        const void* data = nullptr, 
        const int bytes = 0);

    //连接事件通知
    //@sid [out] : 会话ID
    //@e [out] : 错误码
    virtual void fetchXmsClientSessionConnectedNotification(
        const std::string sid, 
        const int e = 0) = 0;

    //发送事件通知
    //@sid [out] : 会话ID
    //@e [out] : 错误码
    //@bytes [out] : 大小
    virtual void fetchXmsClientSessionSentDataEventNotification(
        const std::string sid, 
        const int e = 0, 
        const int bytes = 0) = 0;

    //接收事件通知
    //@sid [out] : 会话ID
    //@e [out] : 错误码
    //@data [out] : 数据
    //@bytes [out] : 大小
    virtual void fetchXmsClientSessionReceivedDataEventNotification(
        const std::string sid, 
        const int e = 0, 
        const void* data = nullptr, 
        const int bytes = 0) = 0;
};//class LibXmsHostClient

#endif//LIB_XMS_HOST_CLIENT_H