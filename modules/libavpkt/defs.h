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

#if defined(_WINDOWS)
#ifdef USE_MODULE_AV_PKT
#define AV_PKT_EXPORT __declspec(dllimport)
#else
#define AV_PKT_EXPORT __declspec(dllexport)
#endif//USE_MODULE_AV_PKT
#elif defined(__linux__)
#define AV_PKT_EXPORT
#endif//_WINDOWS

typedef enum class tagAVMainType_t : uint32_t
{
	AV_MAIN_TYPE_NONE = 0, 
	AV_MAIN_TYPE_STANDARD_H264, 
	AV_MAIN_TYPE_STANDARD_H265, 
	AV_MAIN_TYPE_HK_PRIVATE, 
	AV_MAIN_TYPE_HK_PS, 
	AV_MAIN_TYPE_IMAGE
}AVMainType;

typedef enum class tagAVSubType_t : uint32_t
{
	AV_SUB_TYPE_NONE = 0, 
	AV_SUB_TYPE_VIDEO, 
	AV_SUB_TYPE_AUDIO, 
	AV_SUB_TYPE_IDR, 
	AV_SUB_TYPE_B, 
	AV_SUB_TYPE_P, 
	AV_SUB_TYPE_YUV420P, 
<<<<<<< HEAD
=======
	AV_SUB_TYPE_RGB24, 
>>>>>>> 988daff8f78d5aa9a26ee4853f763da606d880b4
	AV_SUB_TYPE_BGR24, 
	AV_SUB_TYPE_JPEG, 
	AV_SUB_TYPE_PNG, 
	AV_SUB_TYPE_AAC, 
	AV_SUB_TYPE_G711, 
	AV_SUB_TYPE_G722, 
	AV_SUB_TYPE_ADPCM
}AVSubType;

#endif//MODULE_AV_STREAM_AV_PKT_DEFS_H