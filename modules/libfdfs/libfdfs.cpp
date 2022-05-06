#include "boost/make_shared.hpp"
#include "error_code.h"
#include "map/unordered_map.h"
#include "tracker/fdfs_tracker.h"
#include "storage/fdfs_storage.h"
#include "libfdfs.h"
using namespace module::file::storage;

using FdfsStoragePtr = boost::shared_ptr<FdfsStorage>;
using FdfsStorages = UnorderedMap<const uint32_t, FdfsStoragePtr>;
static void* trackerConnectionInfo{nullptr};
static FdfsStorages fdfsStorages;

Libfdfs::Libfdfs()
{}

Libfdfs::~Libfdfs()
{}

int Libfdfs::init(const char* conf /* = nullptr */)
{
	int ret{conf ? Error_Code_Success : Error_Code_Invalid_Param};

	if (Error_Code_Success == ret)
	{
		if (trackerConnectionInfo)
		{
			return Error_Code_Object_Existed;
		}
		
		ret = (!fdfs_client_init(conf) ? Error_Code_Success : Error_Code_Init_Sdk_Failure);

		if (Error_Code_Success == ret)
		{
			trackerConnectionInfo = FdfsTracker().connect();
			ret = (trackerConnectionInfo ? Error_Code_Success : Error_Code_Operate_Failure);
		}
	}

	return ret;
}

int Libfdfs::destroy()
{
	tracker_close_all_connections();
	fdfs_client_destroy();
	return Error_Code_Success;
}

const char* Libfdfs::upload(
	const uint32_t id/* = 0*/, 
	const void* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/, 
	const bool append/* = false*/)
{
	char* ret{nullptr};
	FdfsStoragePtr ptr{fdfsStorages.at(id)};

	if (!ptr)
	{
		ptr = boost::make_shared<FdfsStorage>();
		if (ptr)
		{
			if (Error_Code_Success == ptr->connect((ConnectionInfo*)trackerConnectionInfo))
			{
				fdfsStorages.add(id, ptr);
			}
			else
			{
				ptr.reset();
			}
		}
	}

	if (ptr)
	{
		ret = const_cast<char*>(ptr->upload((ConnectionInfo*)trackerConnectionInfo, data, bytes, append));
	}
	
	return ret;
}

int Libfdfs::download()
{
	return Error_Code_Success;
}
