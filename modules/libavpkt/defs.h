//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-16
//		Description : AV数据包定义
//
//		History:
//					1. 2022-02-16 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PKT_DEFS_H
#define MODULE_AV_STREAM_AV_PKT_DEFS_H

#include <cstdint>

#if defined(WINDOWS)
#ifdef USE_MODULE_AV_PKT
#define AV_PKT_EXPORT __declspec(dllimport)
#else
#define AV_PKT_EXPORT __declspec(dllexport)
#endif//USE_MODULE_AV_PKT
#elif defined(__linux__)
#define AV_PKT_EXPORT
#endif//WINDOWS

typedef enum class tagAVPktType_t : uint32_t
{
	AV_PKT_TYPE_NONE = 0
}AVPktType;

#endif//MODULE_AV_STREAM_AV_PKT_DEFS_H