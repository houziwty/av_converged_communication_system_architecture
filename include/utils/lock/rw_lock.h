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

#ifndef UTILS_LOCK_RW_LOCK_H
#define UTILS_LOCK_RW_LOCK_H

#include "boost/thread/locks.hpp"
#include "boost/thread/shared_mutex.hpp"

namespace utils
{
    namespace lock
    {
        using SharedMutex = boost::shared_mutex;
        using WriteLock = boost::unique_lock<SharedMutex>;
        using ReadLock = boost::shared_lock<SharedMutex>;
    }//namespace lock
}//namespace utils

#endif//UTILS_LOCK_RW_LOCK_H

