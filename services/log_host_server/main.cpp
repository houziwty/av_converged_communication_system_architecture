#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/commandline/commandline.h"
using namespace framework::utils::parser;
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "log_host_server.h"

int main(int argc, char* argv[])
{
    FileLog fileLog;
    fileLog.createNew(argv[0]);
    CommandLineParser parser;
    parser.setOption("xmq_addr", "");
    parser.setOption("xmq_port", "");
    parser.setOption("name", "");
    parser.setOption("expire", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string xmq_addr{parser.getParameter("xmq_addr")};
        const std::string xmq_port{parser.getParameter("xmq_port")};
        const std::string app_name{parser.getParameter("name")};
        const std::string expire_days{parser.getParameter("expire")};

        boost::shared_ptr<XMQNode> node{
            boost::make_shared<LogHostServer>(fileLog, atoi(expire_days.c_str()), argv[0])};
        if(node)
        {
            XMQModeConf conf{0};
            conf.id = 0xB1;
            conf.port = 60531;
            conf.type = XMQModeType::XMQ_MODE_TYPE_DEALER;
            XMem().copy(app_name.c_str(), app_name.length(), conf.name, 128);
            XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);

            if (Error_Code_Success == node->addConf(conf))
            {
                node->run();
                getchar();
                node->stop();
                node->removeConf(conf.id);
            }
        }
    }

    fileLog.destroy();
    return 0;
}