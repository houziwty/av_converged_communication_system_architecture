#include <string>
#include "error_code.h"
#include "fdfs_storage.h"
using namespace module::file::storage;

FdfsStorage::FdfsStorage() : storePathIndex{-1}
{
	memset(groupName, 0, FDFS_GROUP_NAME_MAX_LEN * sizeof(char));
	memset(fileName, 0, FDFS_TRUNK_FILENAME_LENGTH * sizeof(char));
}

FdfsStorage::~FdfsStorage()
{}

int FdfsStorage::connect(const ConnectionInfo* trackerConnectionInfo/* = nullptr*/)
{
	int ret{trackerConnectionInfo ? Error_Code_Success : Error_Code_Invalid_Param};

	if(Error_Code_Success == ret)
	{
		if (!tracker_query_storage_store((ConnectionInfo*)trackerConnectionInfo, &storageConnectionInfo, groupName, &storePathIndex))
		{
			tracker_make_connection(&storageConnectionInfo, &ret);
		}

		ret = (!ret ? Error_Code_Success : Error_Code_Bad_New_Object);
	}

	return ret;
}

const char* FdfsStorage::upload(
	const ConnectionInfo* trackerConnectionInfo/* = nullptr*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	char* ret{nullptr};

	if (trackerConnectionInfo && data && 0 < bytes)
	{
		const std::string fname{fileName};
		if(!fname.empty())
		{
			if (!storage_append_by_filebuff(
				(ConnectionInfo*)trackerConnectionInfo, &storageConnectionInfo, 
				(const char*)data, bytes, groupName, fileName))
			{
				ret = fileName;
			}
		}
		else
		{
			if(!storage_upload_appender_by_filebuff(
				(ConnectionInfo*)trackerConnectionInfo, &storageConnectionInfo, storePathIndex, 
				(const char*)data, bytes, nullptr, nullptr, 0, groupName, fileName))
			{
				ret = fileName;
			}
		}
	}

	return ret;
}
