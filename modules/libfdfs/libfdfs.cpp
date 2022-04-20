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
		ConnectionInfo* tracker{tracker_get_connection()};

		if (tracker)
		{
			ConnectionInfo* pStorageServer;
			ConnectionInfo storageServers[FDFS_MAX_SERVERS_EACH_GROUP];
			ConnectionInfo *pServer;
			ConnectionInfo *pServerEnd;
			int storage_count;
			int store_path_index = 0;
			char group_name[FDFS_GROUP_NAME_MAX_LEN + 1];

			if (!tracker_query_storage_store_list_without_group(
				tracker, storageServers, 
				FDFS_MAX_SERVERS_EACH_GROUP, &storage_count, 
				group_name, &store_path_index))
			{
				printf("tracker_query_storage_store_list_without_group: \n");
				pServerEnd = storageServers + storage_count;
				for (pServer=storageServers; pServer<pServerEnd; pServer++)
				{
					printf("\tserver %d. group_name=%s, " \
						"ip_addr=%s, port=%d\n", \
						(int)(pServer - storageServers) + 1, \
						group_name, pServer->ip_addr, pServer->port);
				}
				printf("\n");
			}

			ConnectionInfo storageServer;
			int result{tracker_query_storage_store(tracker, &storageServer, group_name, &store_path_index)};
			if (0 != result)
			{
				fdfs_client_destroy();
				printf("tracker_query_storage fail, " \
					"error no: %d, error info: %s\n", \
					result, STRERROR(result));
				return result;
			}

			printf("group_name=%s, ip_addr=%s, port=%d\n", \
				group_name, storageServer.ip_addr, \
				storageServer.port);

			if ((pStorageServer=tracker_make_connection(&storageServer, &result)) == NULL)
			{
				fdfs_client_destroy();
				return result;
			}

			int meta_count = 0;
			FDFSMetaData meta_list[32];
			memset(&meta_list, 0, sizeof(meta_list));
			strcpy(meta_list[meta_count].name, "ext_name");
			strcpy(meta_list[meta_count].value, "jpg");
			meta_count++;
			strcpy(meta_list[meta_count].name, "width");
			strcpy(meta_list[meta_count].value, "160");
			meta_count++;
			strcpy(meta_list[meta_count].name, "height");
			strcpy(meta_list[meta_count].value, "80");
			meta_count++;
			strcpy(meta_list[meta_count].name, "file_size");
			strcpy(meta_list[meta_count].value, "115120");
			meta_count++;

			char buf[23981]{0}, name[1024]{0};
			FILE* fd{
				fopen("/mnt/d/av_converged_communication_system_architecture/3rd/fastdfs-master/conf/anti-steal.jpg", "rb+")};
			if (fd)
			{
				fread(buf, 23981, 1, fd);
			}
			
			result = storage_upload_by_filebuff(
				tracker, pStorageServer, store_path_index, 
				buf, 23981, "jpg", 
				meta_list, meta_count, 
				group_name, name);

			FDFSFileInfo file_info;
			result = fdfs_get_file_info(group_name, name, &file_info);

			if (!result)
			{
				printf("file_type:%d, get_from_server:%d, time:%lld, crc32:%d, source_id:%d, file_size:%lld, source_ip_addr:%s.\r\n", 
				file_info.file_type, file_info.get_from_server, file_info.create_timestamp, file_info.crc32, file_info.source_id, file_info.file_size, file_info.source_ip_addr);

				char* buf2 = new char[23981]{0};
				int64_t file_size{23981};
				if (!storage_download_file_to_buff(tracker, pStorageServer, group_name, name, &buf2, &file_size))
				{
					FILE* fd{fopen("/mnt/d/download.jpg", "wb+")};
					fwrite(buf2, file_size, 1, fd);
					fclose(fd);
				}
			}
		}
		else
		{
			destroy();
			ret = Error_Code_Bad_New_Object;
		}
	}
	
	return ret;
}

int Libfdfs::destroy()
{
//	tracker_close_connection_ex(pStorageServer, true);
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
