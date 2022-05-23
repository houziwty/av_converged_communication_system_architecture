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

#ifndef MODULE_DEVICE_DVS_LIB_DEFS_H
#define MODULE_DEVICE_DVS_LIB_DEFS_H

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

typedef enum class tagDVSFactoryType_t : uint32_t
{
	DVS_FACTORY_TYPE_NONE = 0,
	DVS_FACTORY_TYPE_HK, 
	DVS_FACTORY_TYPE_DH,
	DVS_FACTORY_TYPE_UNIVIEW
}DVSFactoryType;

typedef enum class tagDVSModuleType_t : uint32_t
{
	DVS_MODULE_TYPE_NONE = 0,
	DVS_MODULE_TYPE_IPC,
	DVS_MODULE_TYPE_DVR,
	DVS_MODULE_TYPE_NVR,
	DVS_MODULE_TYPE_XVR
}DVSModuleType;

typedef enum class tagDVSConfParamType_t : uint32_t
{
	DVS_CONF_PARAM_TYPE_NONE = 0, 
	DVS_CONF_PARAM_TYPE_CHANNEL_NUMBER				//查询设备通道总数 --> uint32_t
}DVSConfParamType;

//DVS设备配置
typedef struct tagDVSModeConf_t
{
	DVSFactoryType factory;
	DVSModuleType module;
    char user[128];
	char passwd[128];
    char ip[128];
    uint16_t port;
    uint32_t id;                    //物理设备ID标识，0 < id，由调用者定义
}DVSModeConf;

#endif//MODULE_DEVICE_DVS_LIB_DEFS_H