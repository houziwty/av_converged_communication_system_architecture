//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-15
//		Description : AV解析定义
//
//		History:
//					1. 2022-02-15 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PARSER_DEFS_H
#define MODULE_AV_STREAM_AV_PARSER_DEFS_H

#include <cstdint>

#if defined(WINDOWS)
#ifdef USE_MODULE_AV_PARSER
#define AV_PARSER_EXPORT __declspec(dllimport)
#else
#define AV_PARSER_EXPORT __declspec(dllexport)
#endif//USE_MODULE_AV_PARSER
#elif defined(__linux__)
#define AV_PARSER_EXPORT
#endif//WINDOWS

//AV解析角色类型
typedef enum class tagAVParserType_t : int
{
	AV_PARSER_TYPE_NONE = 0,
	AV_PARSER_TYPE_BUFFER_PARSER,
	AV_PARSER_TYPE_PS_PARSER,
}AVParserType;

typedef struct tagAVParserModeConf_t
{
	uint32_t id;                   //>0，由调用者分配
	AVParserType type;
}AVParserModeConf;

#endif//MODULE_AV_STREAM_AV_PARSER_DEFS_H