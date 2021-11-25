//
//		Copyright : @2021, ***, All Rights Reserved
//
//		Author : 王科威
//		E-mail : wangkw531@hotmail.com
//		Date : 2021-11-23
//		Description : 命令行解析类
//
//		History:
//					1. 2021-11-23 由王科威创建
//

#ifndef FRAMEWORK_UTILS_PARSER_COMMAND_LINE_PARSER_H
#define FRAMEWORK_UTILS_PARSER_COMMAND_LINE_PARSER_H

#include <string>
#include "boost/program_options.hpp"

namespace framework
{
	namespace utils
	{
		namespace parser
		{
			class CommandLineParser
			{
			public:
				CommandLineParser(void);
				~CommandLineParser(void);

			public:
				//设置命令参数
				//@option : 参数名
				//@param : 参数值
				//@Return : 错误码
				int setOption(const std::string option, const std::string param);

				//获取参数值
				//@option : 参数名
				//@Return : 参数值
				const std::string getParameter(const std::string option);

				//解析命令
				int parse(int argc = 0, char** argv = nullptr);

			private:
				boost::program_options::options_description desc;
				boost::program_options::variables_map variables;
			};//class CommandLineParser
		}//namespace parser
	}//namespace utils
}//namespace framework

#endif//FRAMEWORK_UTILS_PARSER_COMMAND_LINE_PARSER_H
