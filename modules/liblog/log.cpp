#include "glog/logging.h"
#include "Log.h"

Logger::Logger()
{}

Logger::~Logger()
{}

int Logger::init(const char* name)
{
	printf("Name : %s.\r\n", name);
    google::InitGoogleLogging(name);

    // ...
    LOG(ERROR) << "Found " << name << " cookies";
	
	return 0;
}