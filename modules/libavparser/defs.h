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

typedef enum class tagMainType_t : uint32_t
{
	MAIN_TYPE_NONE = 0,
	MAIN_TYPE_STANDARD_H264,
	MAIN_TYPE_STANDARD_H265,
	MAIN_TYPE_STANDARD_ES,
	MAIN_TYPE_HIKVISION_PRIVATE,
	MAIN_TYPE_HIKVISION_PS,
	MAIN_TYPE_RAW_VIDEO,
	MAIN_TYPE_RAW_AUDIO
}MainType;

typedef enum class tagSubType_t : uint32_t
{
	SUB_TYPE_NONE = 0,
	SUB_TYPE_HEADER_INFO,
	SUB_TYPE_IDR,
	SUB_TYPE_P_FRAME,
	SUB_TYPE_B_FRAME,
	SUB_TYPE_YV12_FRAME,
	SUB_TYPE_YUV420P_FRAME,
	SUB_TYPE_BGR24_FRAME,
	SUB_TYPE_JPEG_FRAME8,
	SUB_TYPE_PNG_FRAME,
	SUB_TYPE_AAC_FRAME,
	SUB_TYPE_G711_FRAME,
	SUB_TYPE_G722_FRAME,
}SubType;

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