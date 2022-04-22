#include <iostream>
#include "error_code.h"
#include "libfdfs.h"
using namespace module::file::storage;

class FdfsUploadBenchmark : public Libfdfs
{
public:
	FdfsUploadBenchmark() : Libfdfs()
	{}
	~FdfsUploadBenchmark() {}
};//FdfsUploadBenchmark

int main(int argc, char* argv[])
{
	FdfsUploadBenchmark benchmark;
	benchmark.init("/mnt/d/av_converged_communication_system_architecture/conf/fastdfs/client.conf");
	benchmark.upload();
	getchar();
	benchmark.destroy();

	return 0;
}