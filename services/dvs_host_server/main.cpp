#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/commandline/commandline.h"
using namespace framework::utils::parser;
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "dvs_host_server.h"

int main(int argc, char* argv[])
{
    FileLog flog;
    flog.createNew(argv[0]);
    CommandLineParser parser;
    parser.setOption("xmq_addr", "");
    parser.setOption("xmq_port", "");
    parser.setOption("xms_port", "");
    parser.setOption("app_name", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string xmq_addr{parser.getParameter("xmq_addr")};
        const std::string xmq_port{parser.getParameter("xmq_port")};
        const std::string xms_port{parser.getParameter("xms_port")};
        const std::string app_name{parser.getParameter("app_name")};

        flog.write(
            SeverityLevel::SEVERITY_LEVEL_INFO, 
            "Parse command line parameters with xmq_addr = [ %s ], xmq_port = [ %s ], xms_port = [ %s ], app_name = [ %s ].",
            xmq_addr.c_str(), xmq_port.c_str(), xms_port.c_str(), app_name.c_str());

        if (!xmq_addr.empty() && !xmq_port.empty() && !xms_port.empty() && !app_name.empty())
        {
            const std::string name{"dvs_host_server"};
            boost::shared_ptr<XMQNode> node{
                boost::make_shared<DvsHostServer>(flog)};
            XMQModeConf conf{0};
            conf.id = 0xB1;
            conf.port = 60531;
            conf.type = XMQModeType::XMQ_MODE_TYPE_DEALER;
            XMem().copy(name.c_str(), name.length(), conf.name, 128);
            XMem().copy(xmq_addr.c_str(), xmq_addr.length(), conf.ip, 32);

            node->addConf(conf);
            node->run();
            //dhs.startXMS(static_cast<unsigned short>(atoi(xms_port.c_str())));
            getchar();
            //dhs.stopXMS(); 
            node->stop();
            node->removeConf(conf.id);
        }
    }
    else
    {
        flog.write(SeverityLevel::SEVERITY_LEVEL_ERROR, "Parse command line failed.");
    }

    flog.destroy();
    return 0;
}