//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-16
//		Description : 数据定义
//
//		History:
//					1. 2022-02-16 由王科威创建
//

#ifndef MODULE_COMMON_DATA_DATA_DEFS_H
#define MODULE_COMMON_DATA_DATA_DEFS_H

#include <cstdint>

#if defined(WINDOWS)
#ifdef USE_MODULE_COMMON_DATA
#define COMMON_DATA_EXPORT __declspec(dllimport)
#else
#define COMMON_DATA_EXPORT __declspec(dllexport)
#endif//USE_MODULE_COMMON_DATA
#elif defined(__linux__)
#define COMMON_DATA_EXPORT
#endif//WINDOWS

typedef enum class tagDataType_t : uint32_t
{
	DATA_TYPE_NONE = 0,
	DATA_TYPE_RAW
}DataType;

#endif//MODULE_COMMON_DATA_DATA_DEFS_H