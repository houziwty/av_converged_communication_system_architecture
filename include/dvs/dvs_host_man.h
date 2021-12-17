//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-12-14
//		Description : 设备主机管理
//
//		History:
//					1. 2021-12-14 由王科威创建
//

#ifndef FRAMEWORK_DVS_HOST_MAN_H
#define FRAMEWORK_DVS_HOST_MAN_H

#include "libdvshost/dvs_host_creator.h"
using namespace module::dvs;
#include "utils/map/unordered_map.h"

namespace framework
{
    namespace dvs
    {
        typedef struct tagDeviceInfo_t
		{
			int userID;
            std::string uuid;
            std::string name;//设备名称
            std::string ip;
			DevicePtr dvs;
            CameraPtrs cameras;
		}DeviceInfo;

        typedef struct tagStreamInfo_t
		{
			int userID;
            int streamID;
            StreamPtr stream;
		}StreamInfo;

        class DvsHostMan
        {
        public:
            DvsHostMan(void);
            ~DvsHostMan(void);

        public:
            //添加
            //@uuid : 设备UUID
            //@devicename [in] : 设备名称
			//@username [in] : 用户名
			//@userpwd [in] : 密码
			//@ip [in] : IP地址
			//@port [in] : 端口号
            //@type [in] : 设备厂商
			//@Return : 错误码
			int addDevice(
                const std::string uuid, 
                const std::string devicename, 
				const std::string username, 
				const std::string userpwd, 
				const std::string ip, 
				const unsigned short port = 0, 
                const FactoryType factory = FactoryType::FACTORY_TYPE_NONE);

			//删除
			//@uuid : 设备UUID
			//@Return : 错误码
			int removeDevice(const std::string uuid);

            //获取设备信息
			//@Return : 设备信息集合
            const std::vector<DeviceInfo> queryDeviceInfos(void);

            //获取设备摄像机信息
            //@uuid : 设备UUID
			//@Return : 摄像机集合
            const CameraPtrs queryCameraPtrs(const std::string uuid);

            //打开实时流
            //@streamID [in] : 流UUID
            //@dvsID [in] : 设备UUID
            //@type [in] : 设备类型
			//@userID [in] : 用户ID
			//@index [in] : 通道索引号
			//@streamType [in] : 码流类型，范围[0, 10]
			//@Return : 流ID
			int openStream(
                const std::string streamID, 
                const std::string dvsID, 
                const FactoryType type = FactoryType::FACTORY_TYPE_NONE, 
				const int userID = -1, 
				const int index = -1, 
				const unsigned int streamType = 0);

			//关闭实时流
			//@streamID [in] : 流ID
			//@Return : 错误码
			int closeStream(const std::string uuid);

        private:
            DvsHostCreator dvsHostCreator;
            UnorderedMap<const std::string, DeviceInfo> dvss;
            UnorderedMap<const std::string, StreamInfo> streams;
        };//class DvsHostMan
    }//namespace dvs
}//namespace framework

#endif//FRAMEWORK_DVS_HOST_MAN_H