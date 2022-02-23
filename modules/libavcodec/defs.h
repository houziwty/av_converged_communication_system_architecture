//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-07
//		Description : AV编解码库定义
//
//		History:
//					1. 2021-12-07 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_CODEC_DEFS_H
#define MODULE_AV_STREAM_AV_CODEC_DEFS_H

#include <cstdint>

#if defined(_WINDOWS)
#ifdef USE_MODULE_AV_CODEC
#define AV_CODEC_EXPORT __declspec(dllimport)
#else
#define AV_CODEC_EXPORT __declspec(dllexport)
#endif//USE_MODULE_AV_CODEC
#elif defined(__linux__)
#define AV_CODEC_EXPORT
#endif//_WINDOWS

//AV编解码类型
typedef enum class tagAVCodecType_t : uint32_t
{
    AV_CODEC_TYPE_NONE = 0,
    AV_CODEC_TYPE_DECODE_H264, 
    AV_CODEC_TYPE_DECODE_H265, 
    AV_CODEC_TYPE_YUV420P_2_BGR24
}AVCodecType;

typedef struct tagAVCodecModeConf_t
{
	uint32_t id;                   //>0，由调用者分配
	AVCodecType type;
}AVCodecModeConf;

#endif//MODULE_AV_STREAM_AV_CODEC_DEFS_H