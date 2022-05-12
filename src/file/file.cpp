#include "boost/checked_delete.hpp"
#include "boost/filesystem.hpp"
#include "error_code.h"
#include "file/file.h"
using namespace framework::utils::file;

Path::Path()
{}

Path::~Path()
{}

const std::string Path::path()
{
	return boost::filesystem::initial_path<boost::filesystem::path>().string();
}

File::File() : fd{nullptr}
{}

File::~File()
{}

int File::read(const char* name, char*& buf, uint64_t& bytes)
{
	int ret{open(name, "rb+")};

	if (Error_Code_Success == ret)
	{
		//按照指定缓存大小读取
#ifdef _WINDOWS
		bytes = fread_s(buf, bytes, bytes, 1, fd);
#else
		bytes = fread(buf, bytes, 1, fd);
#endif//_WINDOWS
	}
	
	return ret;
}

int File::write(
	const char* name/* = nullptr*/, 
	const void* buf/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	int ret{open(name, "wb+")};
	uint64_t retbytes{0};

	if (Error_Code_Success == ret)
	{
		retbytes = fwrite(buf, bytes, 1, fd);
	}
	
	return retbytes == bytes ? Error_Code_Success : Error_Code_Operate_Failure;
}

int File::open(const char* name/* = nullptr*/, const char* mode/* = nullptr*/)
{
	int ret{name && mode ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		close();
		
#ifdef _WINDOWS
		fopen_s(&fd, name, mode);
#else
		fd = fopen(name, mode);
#endif//_WINDOWS

		ret = (fd ? Error_Code_Success : Error_Code_File_Open_Failure);
	}
	
	return ret;
}

int File::close()
{
	int ret{fd ? Error_Code_Success : Error_Code_File_Close_Failure};

	if (Error_Code_Success == ret)
	{
		fclose(fd);
		fd = nullptr;
	}
	
	return ret;
}
