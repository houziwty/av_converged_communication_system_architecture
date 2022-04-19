#include "fastdfs/fdfs_client.h"
#include "error_code.h"
#include "libfdfs.h"
using namespace module::file::storage;

Libfdfs::Libfdfs()
{}

Libfdfs::~Libfdfs()
{}

int Libfdfs::init(const char* conf /* = nullptr */)
{
	int ret{fdfs_client_init(conf)};

	if (!ret)
	{
		/* code */
	}
	
	return ret;
}

int Libfdfs::destroy()
{
	fdfs_client_destroy();
	return Error_Code_Success;
}

int Libfdfs::upload(
	const char* data/* = nullptr*/, 
	const uint64_t bytes/* = 0*/)
{
	return Error_Code_Success;
}

int Libfdfs::download()
{
	return Error_Code_Success;
}
