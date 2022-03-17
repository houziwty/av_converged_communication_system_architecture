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
                //@dir [in] : 日志目录路径
                //@enbaleFile [in] : 写日志文件标识
                //@expireDays [in] : 日志超时删除时间，单位：天，如果0 == expireDays则不删除
                //@Return : 错误码
                //@Commnet : 如果路径存在则按照该路径存储日志文件，
                //           如果路径不存在则在程序当前执行路径下创建存储目录
                int createNew(
                    const char* dir = nullptr, 
                    const bool enableFile = true, 
                    const unsigned int expireDays = 0);

                //销毁
                //@Return : 错误码
                int destroy(void);

                //获取日志文件目录
                //@Return : 目录
                const char* dir(void) const;

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