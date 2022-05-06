//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-19
//		Description : FDFS库
//
//		History:
//					1. 2022-04-19 由王科威创建
//

#ifndef MODULE_FILE_STORAGE_LIB_FDFS_H
#define MODULE_FILE_STORAGE_LIB_FDFS_H

#include <cstdint>

#if defined(_WINDOWS)
#ifdef USE_MODULE_FILE_FDFS
#define FILE_FDFS_EXPORT __declspec(dllimport)
#else
#define FILE_FDFS_EXPORT __declspec(dllexport)
#endif//USE_MODULE_FILE_FDFS
#elif defined(__linux__)
#define FILE_FDFS_EXPORT
#endif//_WINDOWS

namespace module
{
    namespace file
    {
        namespace storage
        {
            class FILE_FDFS_EXPORT Libfdfs
            {
            public:
                Libfdfs(void);
                ~Libfdfs(void);

            public:
                //初始化
                //@dir [in] : 配置文件路径
                //@Return : 错误码
                int init(const char* conf = nullptr);

                //销毁
                //@Return : 错误码
                int destroy(void);

                //块上传
                //@id [in] : 上传ID
                //@data [in] : 数据
                //@bytes [in] : 大小
                //@append [in] : 追加标识
                //@Return : 文件名
                const char* upload(
                    const uint32_t id = 0, 
                    const void* data = nullptr, 
                    const uint64_t bytes = 0, 
                    const bool append = false);

                //下载
                //@id [in] : 下载ID
                //@filename [in] : 文件名
                //@buffer [out] : 数据
                //@bytes [out] : 大小
                //@Return : 错误码
                int download(
                    const uint32_t id = 0, 
                    const char* filename = nullptr, 
                    char* buffer = nullptr, 
                    int64_t* bytes = nullptr);

                //删除
                //@id [in] : 删除ID
                //@filename [in] : 文件名
                //@Return : 错误码
                int remove(
                    const uint32_t id = 0, 
                    const char* filename = nullptr);
            };//class Libfdfs
        }//namespace storage
    }//namespace file
}//namespace module

#endif//MODULE_FILE_STORAGE_LIB_FDFS_H