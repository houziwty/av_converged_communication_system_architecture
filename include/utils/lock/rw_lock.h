//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-15
//		Description : 读写锁
//
//		History: 2021-11-15 由王科威创建
//

#ifndef FRAMEWORK_UTILS_LOCK_RW_LOCK_H
#define FRAMEWORK_UTILS_LOCK_RW_LOCK_H

//#include "boost/thread/locks.hpp"
//#include "boost/thread/shared_mutex.hpp"

#include <mutex>
#include <shared_mutex>

namespace framework
{
    namespace utils
    {
        namespace lock
        {
            using SharedMutex = std::mutex;
 //           using WriteLock = std::unique_lock<SharedMutex>;
//            using ReadLock = std::shared_lock<SharedMutex>;
        }//namespace lock
    }//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_LOCK_RW_LOCK_H

