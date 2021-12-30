//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-29
//		Description : 消息客户端SDK
//
//		History:
//					1. 2021-11-29 由王科威创建
//

#ifndef SERVICE_LIB_XMQ_HOST_CLIENT_H
#define SERVICE_LIB_XMQ_HOST_CLIENT_H

#if defined(_WINDOWS)
#ifdef USE_XMQ_HOST_CLIENT
#define XMQ_HOST_CLIENT_EXPORT __declspec(dllimport)
#else
#define XMQ_HOST_CLIENT_EXPORT __declspec(dllexport)
#endif//USE_DVS_HOST_CLIENT
#elif defined(__linux__)
#define XMQ_HOST_CLIENT_EXPORT
#endif//_WINDOWS

typedef struct tagServiceInfo_t
{
    const char* name;
}ServiceInfo;

class XMQ_HOST_CLIENT_EXPORT LibXmqHostClient
{
public:
    LibXmqHostClient(void);
    virtual ~LibXmqHostClient(void);

public:
    //注册
    //@name [in] : 服务名称
    //@bytes [in] : 大小
    //@ip [in] : IP地址
    //@port [in] : 端口号
    //@Return : 错误码
    //@Comment : 服务名称指调用者自定义的名称，建议使用UUID
    int registerXmqHostClient(
        const void* name = nullptr, 
        const int bytes = 0,  
        const char* ip = nullptr, 
        const unsigned short port = 0);

    //注销
    //@Return : 错误码
    int unregisterXmqHostClient(void);

    //发送
    //@data [in] : 数据
    //@bytes [in] : 大小
    //@Return : 错误码
    int send(const void* data = nullptr, const int bytes = 0);

    //客户端在/离线状态通知
    //@online [out] : true表示在线，false表示离线
    virtual void fetchXmqHostClientOnlineStatusNotification(bool online) = 0;

    //服务在/离线通知
    //@infos [out] : 服务名称集合 
    //@number [out] : 大小
    virtual void fetchXmqHostServiceCapabilitiesNotification(
        const ServiceInfo* infos = nullptr, 
        const int number = 0) = 0;

    //数据接收通知
    //@data [out] : 数据
    //@bytes [out] : 大小
    virtual void fetchXmqHostClientReceivedDataNotification(
        const void* data = nullptr, 
        const int bytes = 0) = 0;
};//class LibXmqHostClient

#endif//SERVICE_LIB_XMQ_HOST_CLIENT_H