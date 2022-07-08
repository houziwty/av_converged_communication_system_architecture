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
//                  2. 2021-12-30 由王科威修改      回调函数定义不能使用typedef void (CALLBACK *func)(...)的形式
//					3. 2022-02-10 由王科威修改      添加服务ID定义
//                  3. 2022-04-07 由王科威修改      添加服务选项定义
//

#ifndef MODULE_NETWORK_XMQ_LIB_DEFS_H
#define MODULE_NETWORK_XMQ_LIB_DEFS_H

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

//XMQ模式类型
typedef enum class tagXMQModuleType_t : int
{
    XMQ_MODULE_TYPE_NONE = 0, 
    XMQ_MODULE_TYPE_DISPATCHER, 
    XMQ_MODULE_TYPE_WORKER, 
    XMQ_MODULE_TYPE_TASK, 
    XMQ_MODULE_TYPE_PUB, 
    XMQ_MODULE_TYPE_SUB
}XMQModuleType;

//XMQ配置选项
typedef struct tagXMQConfOption_t
{
    //  Set high water mark
    //  The option shall set the high water mark for the specified socket. The high water 
    //  mark is a hard limit on the maximum number of outstanding 
    //  messages shall queue in memory for any single peer that the specified socket 
    //  is communicating with.
    uint64_t hwm;

    //  For non-zero values, the lowest bit corresponds to thread 1, second lowest bit 
    //  to thread 2 and so on. For example, a value of 3 specifies that subsequent 
    //  connections on socket shall be handled exclusively by I/O threads 1 and 2.
    uint64_t affinity;

    //  The send buffer option shall set the underlying kernel transmit buffer size for 
    //  the socket to the specified size in bytes. A value of zero means leave the OS 
    //  default unchanged. For details please refer to your operating system documentation 
    //  for the SO_SNDBUF socket option.
    uint64_t sndbuf;

    //  The recv buffer option shall set the underlying kernel receive buffer size for the 
    //  socket to the specified size in bytes. A value of zero means leave the OS default 
    //  unchanged. For details refer to your operating system documentation for the SO_RCVBUF 
    //  socket option.
    uint64_t rcvbuf;

    //  The option shall set the initial reconnection interval for the specified socket. 
    //  The reconnection interval is the period shall wait between attempts to reconnect 
    //  disconnected peers when using connection-oriented transports.
    //  The reconnection interval may be randomized by user to prevent reconnection storms in 
    //  topologies with a large number of peers per socket.
    //  Option value unit	milliseconds
    int32_t reconivl;
}XMQConfOption;

//XMQ节点配置
typedef struct tagXMQNodeConf_t
{
    char name[128];
    char ip[32];
    uint16_t port;
    uint32_t id;                    //节点ID标识，0 < id，由调用者定义
    XMQModuleType type;
    XMQConfOption opt;
}XMQNodeConf;

//服务名称
static const char* XMQHostID = "xmq_host_server";
static const char* DVSHostID = "dvs_host_server";
static const char* HttpHostID = "http_host_server";
static const char* DatabaseHostID = "database_host_server";
static const char* StorageHostID = "storage_host_server";
static const char* LidarHostID = "lidar_host_server";
static const char* SensorHostID = "sensor_host_server";
static const char* SystemInfoHostID = "system_info_host_server";
static const char* IotHostID = "iot_host_server";
static const char* VideoAnalysisHostID = "video_analysis_host_server";
static const char* TrafficCooperateHostID = "traffic_cooperate_host_server";
static const char* LidarPointCloudCacheID = "lidar_point_cloud_cache_server";
static const char* TrafficBreakLawCacheID = "traffic_break_law_cache_server";
static const char* VehicleDriveWarningCacheID = "vehicle_drive_warning_cache_server";
static const char* VehicleDriveTipCacheID = "vehicle_drive_tip_cache_server";

#endif//MODULE_NETWORK_XMQ_LIB_DEFS_H