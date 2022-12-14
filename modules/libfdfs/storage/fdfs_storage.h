//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-22
//		Description : FDFS Storage服务
//
//		History:
//					1. 2022-04-22 由王科威创建
//

#ifndef MODULE_FILE_STORAGE_FDFS_STORAGE_H
#define MODULE_FILE_STORAGE_FDFS_STORAGE_H

#include <cstdint>
#ifdef _WINDOWS
#include "fdfs_client.h"
#else
#include "fastdfs/fdfs_client.h"
#endif//_WINDOWS

namespace module
{
    namespace file
    {
        namespace storage
        {
            class FdfsStorage
            {
            public:
                FdfsStorage(void);
                virtual ~FdfsStorage(void);

            public:
                //连接
                //@trackerConnectionInfo [in] : tacker连接实例
                //@Return : 错误码
                int connect(const ConnectionInfo* trackerConnectionInfo = nullptr);

                //块上传
                //@trackerConnectionInfo [in] : tacker连接实例
                //@data [in] : 数据
                //@bytes [in] : 大小
                //@append [in] : 追加标识
                //@Return : 文件名
                const char* upload(
                    const ConnectionInfo* trackerConnectionInfo = nullptr, 
                    const void* data = nullptr, 
                    const uint64_t bytes = 0, 
                    const bool append = false);

                //文件下载
                //@trackerConnectionInfo [in] : tacker连接实例
                //@filename [in] : 文件名
                //@buffer [out] : 数据
                //@bytes [out] : 大小
                //@Return : 错误码
                int download(
                    const ConnectionInfo* trackerConnectionInfo = nullptr, 
                    const char* filename = nullptr, 
                    char* buffer = nullptr, 
                    int64_t* bytes = nullptr);

                //文件删除
                //@trackerConnectionInfo [in] : tacker连接实例
                //@filename [in] : 文件名
                //@Return ：错误码
                int remove(
                    const ConnectionInfo* trackerConnectionInfo = nullptr, 
                    const char* filename = nullptr);
                    
            private:
                char groupName[FDFS_GROUP_NAME_MAX_LEN];
                char fileName[FDFS_TRUNK_FILENAME_LENGTH];
                int storePathIndex;
                ConnectionInfo storageConnectionInfo;
            };//class FdfsStorage
        }//namespace storage
    }//namespace file
}//namespace module

#endif//MODULE_FILE_STORAGE_FDFS_STORAGE_H