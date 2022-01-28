//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-07
//		Description : ASIO库定义
//
//		History:
//					1. 2021-12-07 由王科威创建
//

#ifndef MODULE_NETWORK_ASIO_DEFS_H
#define MODULE_NETWORK_ASIO_DEFS_H

#include <cstdint>

#if defined(_WINDOWS)
#ifdef USE_MODULE_NETWORK_ASIO
#define NETWORK_ASIO_EXPORT __declspec(dllimport)
#else
#define NETWORK_ASIO_EXPORT __declspec(dllexport)
#endif//USE_MODULE_NETWORK_ASIO
#elif defined(__linux__)
#define NETWORK_ASIO_EXPORT
#endif//_WINDOWS

//ASIO角色类型
typedef enum class tagASIOModeType_t : int
{
    ASIO_MODE_TYPE_NONE = 0,
    ASIO_MODE_TYPE_LISTEN,
    ASIO_MODE_TYPE_CONNECT
}ASIOModeType;

//ASIO协议类型
typedef enum class tagASIOProtoType_t : int
{
    ASIO_PROTO_TYPE_NONE = 0,
    ASIO_PROTO_TYPE_TCP,
    ASIO_PROTO_TYPE_UDP
}ASIOProtoType;

//ASIO角色配置
typedef struct tagASIOModeConf_t
{
    char ip[32];
    uint16_t port;
    uint32_t id;                    //角色ID标识，0 < id，由调用者定义
    ASIOModeType mode;              //仅ASIO_PROTO_TYPE_TCP == proto时有效
    ASIOProtoType proto;
}ASIOModeConf;

#endif//MODULE_NETWORK_ASIO_DEFS_H