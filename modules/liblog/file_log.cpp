#include <cstdarg>
#include <string>
#include "glog/logging.h"
#include "boost/filesystem.hpp"
#include "error_code.h"
#include "file_log.h"
using namespace module::file::log;

static std::string logDir{""};

FileLog::FileLog()
{}

FileLog::~FileLog()
{}

int FileLog::createNew(
	const char* dir /* = nullptr */, 
	const bool enableFile /* = true */,
	const unsigned int expireDays /* = 0 */)
{
	//The dirctory must be existed.
	logDir.append(dir ? dir : "");

	if (enableFile)
	{
		if (!boost::filesystem::exists(logDir))
		{
			logDir = boost::filesystem::initial_path<boost::filesystem::path>().string();
		}

#ifdef _WINDOWS
		logDir += "\\log";

#else
		logDir += "/log";
#endif//_WINDOWS

		boost::filesystem::create_directory(logDir);
		FLAGS_log_dir = logDir;
	}
	else
	{
		FLAGS_logtostderr = true;
	}

	FLAGS_stderrthreshold = google::GLOG_INFO;
	FLAGS_colorlogtostderr = true;
	google::InitGoogleLogging("");
	if (0 < expireDays)
	{
		google::EnableLogCleaner(expireDays);
	}
	
	return Error_Code_Success;
}

int FileLog::destroy()
{
	google::DisableLogCleaner();
	google::ShutdownGoogleLogging();
	return Error_Code_Success;
}

const char* FileLog::dir() const
{
	return logDir.c_str();
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
