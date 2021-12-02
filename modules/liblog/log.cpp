#include <cstdarg>
#include "glog/logging.h"
#include "error_code.h"
#include "log.h"
using namespace module::file::log;

FileLog::FileLog()
{}

FileLog::~FileLog()
{}

int FileLog::createNew(const std::string path)
{
	int ret{!path.empty() ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
#ifdef WINDOWS
		std::size_t pos{path.rfind('\\')};
		const std::string dir{path.substr(0, pos) + "\\log"};
#else
		std::size_t pos{path.rfind('/')};
		const std::string dir{path.substr(0, pos) + "/log"};
#endif
		
		FLAGS_stderrthreshold = google::INFO;
		FLAGS_colorlogtostderr = 1;
		google::InitGoogleLogging(path.c_str());
		google::SetLogDestination(google::INFO, dir.c_str());
	}
	
	return ret;
}

int FileLog::destroy()
{
	google::ShutdownGoogleLogging();
	return Error_Code_Success;
}

int FileLog::write(const SeverityLevel severity, const std::string fmt, ...)
{
	char text[4096]{0};
	va_list list;
	va_start(list, fmt.c_str());
	vsnprintf(text, 4096, fmt.c_str(), list);
	va_end(list);

	int ret{Error_Code_Success};

	if (SeverityLevel::SEVERITY_LEVEL_INFO == severity)
	{
		LOG(INFO) << text;
	}
	else if (SeverityLevel::SEVERITY_LEVEL_WARNING == severity)
	{
		LOG(WARNING) << text;
	}
	else if (SeverityLevel::SEVERITY_LEVEL_ERROR == severity)
	{
		LOG(ERROR) << text;
	}
	else
	{
		ret = Error_Code_Operate_Not_Support;
	}

	return ret;
}