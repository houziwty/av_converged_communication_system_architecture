#include "error_code.h"
#include "utils/commandline/commandline.h"
using namespace framework::utils::parser;

CommandLineParser::CommandLineParser()
{}

CommandLineParser::~CommandLineParser()
{}

int CommandLineParser::setOption(const std::string option, const std::string param)
{
	int ret{ !option.empty() && !param.empty() ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		desc.add_options()(option.c_str(), boost::program_options::value<std::string>(), param.c_str());
	}

	return ret;
}

const std::string CommandLineParser::getParameter(const std::string option)
{
	std::string param;

	if (!option.empty() && 0 < variables.count(option))
	{
		param = variables[option].as<std::string>();
	}

	return std::move(param);
}

int CommandLineParser::parse(int argc, char** argv)
{
	int ret{ 0 < argc && argv ? Error_Code_Success : Error_Code_Invalid_Param };

	if (Error_Code_Success == ret)
	{
		boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), variables);
	}

	return ret;
}
