//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : Defs
//
//		History:
//					1. 2021-11-15 由王科威创建
//                  2. 2021-12-30 由王科威修改  回调函数定义不能使用typedef void (CALLBACK *func)(...)的形式
//					3. 2022-02-10 由王科威修改 添加服务ID
//

#ifndef MODULE_NETWORK_XMQ_DEFS_H
#define MODULE_NETWORK_XMQ_DEFS_H

#include <cstdint>

#if defined(_WINDOWS)
#define CALLBACK __stdcall
#ifdef USE_MODULE_NETWORK_XMQ
#define NETWORK_XMQ_EXPORT __declspec(dllimport)
#else
#define NETWORK_XMQ_EXPORT __declspec(dllexport)
#endif//USE_MODULE_NETWORK_XMQ
#elif defined(__linux__)
#define NETWORK_XMQ_EXPORT
#define CALLBACK
#endif//_WINDOWS

typedef void* ctx_t;
typedef void* socket_t;
typedef void* _thread_t;

//XMQ角色类型
typedef enum class tagXMQModeType_t : int
{
    XMQ_MODE_TYPE_NONE = 0,
    XMQ_MODE_TYPE_ROUTER,
    XMQ_MODE_TYPE_DEALER,
    XMQ_MODE_TYPE_PUB,
    XMQ_MODE_TYPE_SUB
}XMQModeType;

//XMQ角色配置
typedef struct tagXMQModeConf_t
{
    char name[128];
    char ip[32];
    uint16_t port;
    uint32_t id;                    //角色ID标识，0 < id，由调用者定义
    XMQModeType type;
}XMQModeConf;

//服务信息
typedef struct tagServiceInfo_t
{
    char name[128];
}ServiceInfo;

static const char* XMQHostID = "xmq_host_server";
static const char* DVSHostID = "dvs_host_server";
static const char* DatabaseHostID = "database_host_server";
static const char* LidarHostID = "lidar_host_server";
static const char* SensorHostID = "sensor_host_server";
static const char* VideoAnalysisHostID = "video_analysis_host_server";
static const char* TrafficCooperateHostID = "traffic_cooperate_host_server";
static const char* LidarPointCloudCacheID = "lidar_point_cloud_cache_server";
static const char* TrafficBreakLawCacheID = "traffic_break_law_cache_server";
static const char* VehicleDriveWarningCacheID = "vehicle_drive_warning_cache_server";
static const char* VehicleDriveTipCacheID = "vehicle_drive_tip_cache_server";

#endif//MODULE_NETWORK_XMQ_DEFS_H