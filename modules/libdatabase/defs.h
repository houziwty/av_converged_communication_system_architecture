//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-01-27
//		Description : Defs
//
//		History:
//					1. 2022-01-27 由王科威创建
//

#ifndef MODULE_FILE_DATABASE_DATABASE_NODE_DEFS_H
#define MODULE_FILE_DATABASE_DATABASE_NODE_DEFS_H

#include <cstdint>

#if defined(_WINDOWS)
#ifdef USE_MODULE_FILE_DATABASE
#define FILE_DATABASE_EXPORT __declspec(dllimport)
#else
#define FILE_DATABASE_EXPORT __declspec(dllexport)
#endif//USE_MODULE_FILE_DATABASE
#elif defined(__linux__)
#define FILE_DATABASE_EXPORT
#endif//_WINDOWS

typedef enum tagDatabaseType_t : int
{
	DATABASE_TYPE_NONE = 0,
	DATABASE_TYPE_REDIS, 
	DATABASE_TYPE_MYSQL
}DatabaseType;

//数据库配置
typedef struct tagDBModeConf_t
{
	DatabaseType type;
	char passwd[64];
    char ip[128];
    uint16_t port;
    uint32_t id;                    //数据库ID标识，0 < id，由调用者定义
}DBModeConf;

#endif//MODULE_FILE_DATABASE_DATABASE_NODE_DEFS_H