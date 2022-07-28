#include "error_code.h"
#include "commandline/commandline.h"
using namespace framework::utils::parser;
#include "memory/xstr.h"
using namespace framework::utils::memory;
#include "server.h"

int main(int argc, char* argv[])
{
    FileLog log;
    log.createNew("", false);
    CommandLineParser parser;
    parser.setOption("xmq_addr", "");
    parser.setOption("xmq_port", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string xmq_addr{parser.getParameter("xmq_addr")};
        const std::string xmq_port{parser.getParameter("xmq_port")};
        const std::string name{HttpFlvHostID};
        XMQNodeConf conf{0};
        conf.id = 0xB1;
        conf.port = atoi(xmq_port.c_str());
        conf.type = XMQModuleType::XMQ_MODULE_TYPE_TASK;
		XStr().copy(name.c_str(), name.length(), conf.name, 128);
		XStr().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);

        Server svr(log);
        svr.run(conf);
        getchar();
        svr.stop();
    }

    log.destroy();
    return 0;
}