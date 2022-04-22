#include "fastdfs/fdfs_client.h"
#include "fdfs_tracker.h"
using namespace module::file::storage;

FdfsTracker::FdfsTracker()
{}

FdfsTracker::~FdfsTracker()
{}

void* FdfsTracker::connect()
{
	return tracker_get_connection();
}
