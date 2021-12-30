//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : Defs
//
//		History:
//					1. 2021-11-15 由王科威创建
//                  2. 2021-12-30 由王科威修改  回调函数定义不能使用typedef void (CALLBACK *func)(...)的形式
//

#ifndef MODULE_NETWORK_XMQ_DEFS_H
#define MODULE_NETWORK_XMQ_DEFS_H

#if defined(_WINDOWS)
#define CALLBACK __stdcall
#ifdef USE_MODULE_NETWORK_XMQ
#define NETWORK_XMQ_EXPORT __declspec(dllimport)
#else
#define NETWORK_XMQ_EXPORT __declspec(dllexport)
#endif//USE_MODULE_NETWORK_XMQ
#elif defined(__linux__)
#define NETWORK_XMQ_EXPORT
#define CALLBACK
#endif//_WINDOWS

#include <functional>

typedef void* ctx_t;
typedef void* socket_t;

//数据回调函数
//_1 [out] : 标识
//_2 [out] : 大小
//_3 [out] : 数据
//_4 [out] : 大小
typedef std::function<void(const void*, const int, const void*, const int)> PolledDataWithIDCallback;

//数据回调函数
//_1 [out] : 标识
//_2 [out] : 大小
typedef std::function<void(const void*, const int)> PolledDataWithoutIDCallback;

#endif//MODULE_NETWORK_XMQ_DEFS_H