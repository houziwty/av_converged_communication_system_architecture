//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-07
//		Description : HTTP库定义
//
//		History:
//					1. 2021-12-07 由王科威创建
//

#ifndef MODULE_NETWORK_HTTP_DEFS_H
#define MODULE_NETWORK_HTTP_DEFS_H

#include <cstdint>

#if defined(_WINDOWS)
#ifdef USE_MODULE_NETWORK_HTTP
#define NETWORK_HTTP_EXPORT __declspec(dllimport)
#else
#define NETWORK_HTTP_EXPORT __declspec(dllexport)
#endif//USE_MODULE_NETWORK_HTTP
#elif defined(__linux__)
#define NETWORK_HTTP_EXPORT
#endif//_WINDOWS

//HTTP角色配置
typedef struct tagHTTPModeConf_t
{
    uint32_t id;                   //>0，由调用者分配
}HTTPModeConf;

#endif//MODULE_NETWORK_HTTP_DEFS_H