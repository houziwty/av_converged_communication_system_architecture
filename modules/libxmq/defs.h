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
//

#ifndef MODULE_NETWORK_XMQ_DEFS_H
#define MODULE_NETWORK_XMQ_DEFS_H

#include <string>
#include "boost/function.hpp"

#if defined(WINDOWS)
#ifdef USE_MODULE_NETWORK_XMQ
#define NETWORK_XMQ_EXPORT __declspec(dllimport)
#else
#define NETWORK_XMQ_EXPORT __declspec(dllexport)
#endif//USE_MODULE_NETWORK_XMQ
#elif defined(__linux__)
#define NETWORK_XMQ_EXPORT
#endif//WINDOWS

typedef void ctx_t;
typedef void socket_t;
typedef boost::function<void(const std::string, const std::string)> PolledDataWithIDCallback;
typedef boost::function<void(const std::string)> PolledDataWithoutIDCallback;

#endif//MODULE_NETWORK_XMQ_DEFS_H