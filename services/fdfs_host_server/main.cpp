#include "error_code.h"
#include "commandline/commandline.h"
using namespace framework::utils::parser;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "fdfs_host_server.h"

int main(int argc, char* argv[])
{
    CommandLineParser parser;
    parser.setOption("xmq_addr", "");
    parser.setOption("xmq_port", "");
    parser.setOption("conf", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string xmq_addr{parser.getParameter("xmq_addr")};
        const std::string xmq_port{parser.getParameter("xmq_port")};
        const std::string conf_dir{parser.getParameter("conf")};
		const std::string name{"fastdfs_host_server"};

		FileLog log;
		log.createNew(nullptr, false);

		XMQNodeConf conf{ 0 };
		conf.id = 0xB1;
		conf.port = atoi(xmq_port.c_str());
		conf.type = XMQModuleType::XMQ_MODULE_TYPE_TASK;
		XMem().copy(name.c_str(), name.length(), conf.name, 128);
		XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);

		FastDFSHostServer svr{conf, log, conf_dir.c_str()};
		int ret{svr.addNode(conf)};

        if(Error_Code_Success == ret)
        {
			log.write(
				SeverityLevel::SEVERITY_LEVEL_INFO,
				"Run fast dfs host server [ %s ] with xmq server [ %s ], port [ %s ] successfully.",
				name.c_str(), xmq_addr.c_str(), xmq_port.c_str());
			getchar();
			svr.removeNode(conf.id);
        }
		else
		{
			log.write(
				SeverityLevel::SEVERITY_LEVEL_ERROR,
				"Run fast dfs host server [ %s ] with xmq server [ %s ], port [ %s ] failed, result = [ %d ].",
				name.c_str(), xmq_addr.c_str(), xmq_port.c_str(), ret);
		}
    }

    return 0;
}