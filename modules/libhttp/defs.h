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

typedef enum tagHttpValueType_t : uint32_t
{
    HTTP_VALUE_TYPE_NONE = 0,
    HTTP_VALUE_TYPE_INTEGER, 
    HTTP_VALUE_TYPE_STRING
}HttpValueType;

typedef struct tagHttpResponseHeader_t
{
    HttpValueType type;
    const char* name;
    union
    {
        const char* c_value;
        int32_t i_value;
    };
}HttpResponseHeader;

#endif//MODULE_NETWORK_HTTP_DEFS_H