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

#ifndef MODULE_DEVICE_DVS_DEFS_H
#define MODULE_DEVICE_DVS_DEFS_H

#include <cstdint>

#if defined(_WINDOWS)
#define CALLBACK __stdcall
#ifdef USE_MODULE_DEVICE_DVS
#define DEVICE_DVS_EXPORT __declspec(dllimport)
#else
#define DEVICE_DVS_EXPORT __declspec(dllexport)
#endif//USE_MODULE_DEVICE_DVS
#elif defined(__linux__)
#define DEVICE_DVS_EXPORT
#define CALLBACK
#endif//_WINDOWS

typedef void* _thread_t;

typedef enum class tagDVSFactoryType_t : uint32_t
{
	DVS_FACTORY_TYPE_NONE = 0,
	DVS_FACTORY_TYPE_HK
}DVSFactoryType;

typedef enum class tagDVSModelType_t : uint32_t
{
	DVS_MODEL_TYPE_NONE = 0,
	DVS_MODEL_TYPE_IPC
}DVSModelType;

//DVS设备配置
typedef struct tagDVSModeConf_t
{
	char name[128];
    char user[128];
	char passwd[64];
    char ip[128];
    uint16_t port;
    uint32_t id;                    //物理设备ID标识，0 < id，由调用者定义
	uint32_t channels;				//设备通道个数，仅设备查询有效
    DVSFactoryType factory;
	DVSModelType model;
}DVSModeConf;

#endif//MODULE_DEVICE_DVS_DEFS_H