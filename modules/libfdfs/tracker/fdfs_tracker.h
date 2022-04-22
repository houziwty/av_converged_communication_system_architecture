//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-04-22
//		Description : FDFS Tracker服务
//
//		History:
//					1. 2022-04-22 由王科威创建
//

#ifndef MODULE_FILE_STORAGE_FDFS_TRACKER_H
#define MODULE_FILE_STORAGE_FDFS_TRACKER_H

namespace module
{
    namespace file
    {
        namespace storage
        {
            class FdfsTracker
            {
            public:
                FdfsTracker(void);
                virtual ~FdfsTracker(void);

            public:
                //连接
                //@Return : 连接实例
                void* connect(void);
            };//class FdfsTracker
        }//namespace storage
    }//namespace file
}//namespace module

#endif//MODULE_FILE_STORAGE_FDFS_TRACKER_H