//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2022-05-12
//		Description : 文件操作
//
//		History:
//					1. 2022-05-12 由王科威创建
//

#ifndef FRAMEWORK_UTILS_FILE_FILE_AND_PATH_H
#define FRAMEWORK_UTILS_FILE_FILE_AND_PATH_H

#include <stdint.h>
#include <string.h>

namespace framework
{
	namespace utils
	{
		namespace file
		{
			class Path
			{
			public:
				Path(void);
				virtual ~Path(void);

			public:
				//获取路径
				//@Return : 路径
				const std::string path(void);
			};//class Path

			class File
			{
			public:
				File(void);
				virtual ~File(void);

			public:
				//读
				//@name [in] : 文件名
				//@buf [in,out] : 数据
				//@bytes [in,out] : 大小
				//@Return : 错误码
				int read(
					const char* name, 
					char* buf, 
					uint64_t& bytes);

				//写
				//@name [in] : 文件名
				//@buf [in] : 数据
				//@bytes [in] : 大小
				//@Return : 错误码
				int write(
					const char* name = nullptr, 
					const void* buf = nullptr, 
					const uint64_t bytes = 0);

			private:
				int open(const char* name = nullptr, const char* mode = nullptr);
				int close(void);

			private:
				FILE* fd;
			};//class File
		}//namespace file
	}//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_FILE_FILE_AND_PATH_H
