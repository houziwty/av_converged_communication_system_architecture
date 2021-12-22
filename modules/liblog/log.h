//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-25
//		Description : 日志类
//
//		History:
//					1. 2021-11-25 由王科威创建
//

#ifndef MODULE_FILE_LOG_FILELOG_H
#define MODULE_FILE_LOG_FILELOG_H

#include <string>

#if defined(_WINDOWS)
#ifdef USE_MODULE_FILE_LOG
#define FILE_LOG_EXPORT __declspec(dllimport)
#else
#define FILE_LOG_EXPORT __declspec(dllexport)
#endif//USE_MODULE_FILE_LOG
#elif defined(__linux__)
#define FILE_LOG_EXPORT
#endif//_WINDOWS

namespace module
{
    namespace file
    {
        namespace log
        {
            typedef enum class tagSeverityLevel_t : int
            {
                SEVERITY_LEVEL_INFO = 0,
                SEVERITY_LEVEL_WARNING,
                SEVERITY_LEVEL_ERROR
            }SeverityLevel;

            class FILE_LOG_EXPORT FileLog
            {
            public:
                FileLog(void);
                ~FileLog(void);

            public:
                //创建
                //@path [in] : 应用程序路径，通常为argv[0]
                //@Return : 错误码
                int createNew(const std::string path);

                //销毁
                //@Return : 错误码
                int destroy(void);

                //写入
                //@level : 等级
                //@fmt : 格式
                //@... : 参数列表
                //@Return : 错误码
                //@Comment : Windows下fmt不能使用std::string
                int write(const SeverityLevel severity, const char* fmt, ...);
            };//class FileLog
        }//namespace log
    }//namespace file
}//namespace module

#endif//MODULE_FILE_LOG_FILELOG_H