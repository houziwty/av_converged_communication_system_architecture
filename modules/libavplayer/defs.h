//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-02-21
//		Description : AV播放定义
//
//		History:
//					1. 2022-02-21 由王科威创建
//

#ifndef MODULE_AV_STREAM_AV_PLAYER_DEFS_H
#define MODULE_AV_STREAM_AV_PLAYER_DEFS_H

#include <cstdint>

#if defined(_WINDOWS)
#ifdef USE_MODULE_AV_PLAYER
#define AV_PLAYER_EXPORT __declspec(dllimport)
#else
#define AV_PLAYER_EXPORT __declspec(dllexport)
#endif//USE_MODULE_AV_PLAYER
#elif defined(__linux__)
#define AV_PLAYER_EXPORT
#endif//_WINDOWS

typedef enum tagAVPlayerType_t : uint32_t
{
	AV_PLAYER_TYPE_NONE = 0,
	AV_PLAYER_TYPE_D3D, 
	AV_PLAYER_TYPE_AAC
}AVPlayerType;

typedef struct tagAVPlayerModeConf_t
{
	uint32_t id;                   		//>0，由调用者分配
	AVPlayerType type;
	union
	{
		struct
		{
			void* hwnd;					//视频播放窗口
			void* areas;				//区域框参数
		}video;
		struct
		{
			/* data */
		}audio;
	};
}AVPlayerModeConf;

#endif//MODULE_AV_STREAM_AV_PLAYER_DEFS_H