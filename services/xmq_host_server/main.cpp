#include <iostream>
#include "boost/make_shared.hpp"
#include "error_code.h"
#include "utils/commandline/commandline.h"
using namespace framework::utils::parser;
#include "utils/memory/xmem.h"
using namespace framework::utils::memory;
#include "xmq_host_server.h"

int main(int argc, char* argv[])
{
    FileLog fileLog;
    fileLog.createNew(argv[0]);
    CommandLineParser parser;
    parser.setOption("local_port", "");
    // parser.setOption("pub_port", "");
    // parser.setOption("app_name", "");

    if (Error_Code_Success == parser.parse(argc, argv))
    {
        const std::string local_port{parser.getParameter("local_port")};
        // const std::string pub_port{parser.getParameter("pub_port")};
        // const std::string app_name{parser.getParameter("app_name")};

        boost::shared_ptr<XMQNode> node{
            boost::make_shared<XmqHostServer>(fileLog)};
        if (node)
        {
            const std::string name{XMQHostID};
            XMQModeConf conf{0};
            conf.id = 0xA1;
            conf.port = atoi(local_port.c_str());
            conf.type = XMQModeType::XMQ_MODE_TYPE_ROUTER;
            XMem().copy(name.c_str(), name.length(), conf.name, 128);

            fileLog.write(
                SeverityLevel::SEVERITY_LEVEL_INFO, 
                "Run xmq host server with name = [ %s ], local_port = [ %d ], id = [ %x ], type = [ %d ].",
                conf.name, conf.port, conf.id, static_cast<int>(conf.type));
            
            if (Error_Code_Success == node->addConf(conf))
            {
                node->run();
                getchar();
                node->stop();
            }
        }
        else
        {
            fileLog.write(SeverityLevel::SEVERITY_LEVEL_ERROR, "Create instance of node failed.");
        }
    }
    else
    {
        fileLog.write(SeverityLevel::SEVERITY_LEVEL_ERROR, "Parse command line failed.");
    }

    fileLog.destroy();
    return 0;
}