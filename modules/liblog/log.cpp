#include <cstdarg>
#include <string>
#include "glog/logging.h"
#include "error_code.h"
#include "log.h"
using namespace module::file::log;

FileLog::FileLog()
{}

FileLog::~FileLog()
{}

int FileLog::createNew(const char* path /* = nullptr*/)
{
	int ret{path ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		std::size_t slashPos{ 0 };
		std::string dir;
		const std::string pathname{ path };

#ifdef _WINDOWS
		slashPos = pathname.rfind('\\');
		dir = pathname.substr(0, slashPos) + "\\log";
#else
		slashPos = pathname.rfind('/');
		dir = pathname.substr(0, slashPos) + "/log";
#endif//_WINDOWS
		
		FLAGS_stderrthreshold = google::GLOG_INFO;
		FLAGS_colorlogtostderr = true;
		FLAGS_log_dir = dir;
		google::InitGoogleLogging(path);
//		google::SetLogDestination(google::GLOG_INFO, dir.c_str());
	}
	
	return ret;
}

int FileLog::destroy()
{
	google::ShutdownGoogleLogging();
	return Error_Code_Success;
}

int FileLog::write(const SeverityLevel severity, const char* fmt, ...)
{
	char text[4096]{0};
	va_list list;
	va_start(list, fmt);
	vsnprintf(text, 4096, fmt, list);
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
