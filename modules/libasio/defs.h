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

#if defined(_WINDOWS)
#ifdef USE_MODULE_NETWORK_ASIO
#define NETWORK_ASIO_EXPORT __declspec(dllimport)
#else
#define NETWORK_ASIO_EXPORT __declspec(dllexport)
#endif//USE_MODULE_NETWORK_ASIO
#elif defined(__linux__)
#define NETWORK_ASIO_EXPORT
#endif//_WINDOWS

#include "boost/asio.hpp"
#include "boost/enable_shared_from_this.hpp"
#include "boost/function.hpp"
#include "boost/thread.hpp"

#endif//MODULE_NETWORK_ASIO_DEFS_H