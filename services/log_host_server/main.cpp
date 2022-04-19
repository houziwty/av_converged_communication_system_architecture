#include "error_code.h"
#include "commandline/commandline.h"
using namespace framework::utils::parser;
#include "memory/xmem.h"
using namespace framework::utils::memory;
#include "log_host_server.h"

int main(int argc, char* argv[])
{
    CommandLineParser parser;
    parser.setOption("xmq_addr", "");
    parser.setOption("xmq_port", "");
    parser.setOption("name", "");
    parser.setOption("file_dir", "");
    parser.setOption("expire", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string xmq_addr{parser.getParameter("xmq_addr")};
        const std::string xmq_port{parser.getParameter("xmq_port")};
        const std::string log_name{parser.getParameter("name")};
        const std::string file_dir{parser.getParameter("file_dir")};
        const std::string expire_days{parser.getParameter("expire")};

		FileLog log;
		log.createNew(file_dir.c_str(), true, atoi(expire_days.c_str()));

		XMQNodeConf conf{ 0 };
		conf.id = 0xB1;
		conf.port = atoi(xmq_port.c_str());
		conf.type = XMQModuleType::XMQ_MODULE_TYPE_TASK;
		XMem().copy(log_name.c_str(), log_name.length(), conf.name, 128);
		XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);

		LogHostServer svr{conf, log, (uint32_t)atoi(expire_days.c_str())};

        if(Error_Code_Success == svr.addNode(conf))
        {
			log.write(
				SeverityLevel::SEVERITY_LEVEL_INFO,
				"Run log host server [ %s ] with xmq server [ %s ], port [ %s ], log directory [ %s ], expire days [ %s ] successfully.",
				log_name.c_str(), xmq_addr.c_str(), xmq_port.c_str(), log.dir(), expire_days.c_str());
			getchar();
			svr.removeNode(conf.id);
        }
		else
		{
			log.write(
				SeverityLevel::SEVERITY_LEVEL_ERROR,
				"Run log host server [ %s ] with xmq server [ %s ], port [ %s ], log directory [ %s ], expire days [ %s ] failed.",
				log_name.c_str(), xmq_addr.c_str(), xmq_port.c_str(), log.dir(), expire_days.c_str());
		}
    }

    return 0;
}